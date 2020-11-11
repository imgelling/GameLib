#include "GameNetwork.h"
#include <iostream>
#include <deque>
///////////////// THREADPOOL

void ThreadPool::Stop()
{
	if (HasStopped())
	{
#ifdef _DEBUG
		global_stream_lock.lock();
		std::cout << "Attempted to Stop while service was stopped." << std::endl;
		global_stream_lock.unlock();
#endif
		return;
	}

	stop_mutex.lock();
	stopped = true;
	stop_mutex.unlock();
	io_service->stop();
	worker_threads.join_all();  // These were causing the hive to never stop
	work.reset();				// and this one, I moved them after io_service->stop(), seems to be fixed, keep an eye out

}

void ThreadPool::Reset()
{
	if (!HasStopped())
	{
#ifdef _DEBUG
		global_stream_lock.lock();
		std::cout << "Attempted to Reset ThreadPool while it has not been stopped." << std::endl;
		global_stream_lock.unlock();
#endif
		return;
	}

	io_service.reset();
	work.reset();
}

bool ThreadPool::HasStopped()
{
	stop_mutex.lock();
	bool ret = stopped;
	stop_mutex.unlock();
	return ret;
}

void ThreadPool::WorkerThread(boost::shared_ptr< boost::asio::io_service > io_service)
{
//#ifdef _DEBUG
//	global_stream_lock.lock();
//	std::cout << "[" << boost::this_thread::get_id()
//		<< "] Thread Start" << std::endl;
//	global_stream_lock.unlock();
//#endif
	bool running = true;

	while (running)
	{
		try
		{
			boost::system::error_code ec;
			io_service->run(ec);
			if (ec)
			{
				global_stream_lock.lock();
				std::cout << "[" << boost::this_thread::get_id()
					<< "] Error: " << ec << std::endl;
				global_stream_lock.unlock();
			}
			break;
		}
		catch (std::exception & ex)
		{
			global_stream_lock.lock();
			std::cout << "[" << boost::this_thread::get_id()
				<< "] Exception: " << ex.what() << std::endl;

			global_stream_lock.unlock();
		}
		
		if (stopped)
		{
			stop_mutex.lock();
			running = false;
			stop_mutex.unlock();
		}
	}

//#ifdef _DEBUG
//	global_stream_lock.lock();
//	std::cout << "[" << boost::this_thread::get_id()
//		<< "] Thread Finish" << std::endl;
//	global_stream_lock.unlock();
//#endif
}

ThreadPool::ThreadPool(int NumOfThreads)
{
	numOfThreads = NumOfThreads;
	stopped = true;
}

void ThreadPool::Start()
{
	if (!HasStopped())
	{
#ifdef _DEBUG
		global_stream_lock.lock();
		std::cout << "Attempted to Start ThreadPool while it is already started." << std::endl;
		global_stream_lock.unlock();
#endif
		return;
	}
	try
	{
		stop_mutex.lock();
		stopped = false;
		stop_mutex.unlock();
		io_service = boost::shared_ptr<boost::asio::io_service>(new boost::asio::io_service);
		work = boost::shared_ptr<boost::asio::io_service::work>(new boost::asio::io_service::work(*io_service));
		for (int x = 0; x < numOfThreads; x++)
		{
			worker_threads.create_thread(boost::bind(&ThreadPool::WorkerThread, this, io_service));
		}

	}
	catch (std::exception& ex)
	{
		global_stream_lock.lock();
		std::cout << "Exception in " << __FUNCTION__ << ": " << ex.what() << std::endl;
		global_stream_lock.unlock();
	}
}

///////////////// TCP_SERVER

void tcp_server::handle_accept(GameConnection::pointer new_connection, const boost::system::error_code& error)
{
	OnAccept(new_connection, error);

	// Start another ansyc accept
	if (!stopped_)
		start_accept();
}

tcp_server::tcp_server(boost::asio::io_service& io_service, unsigned int port)
	: acceptor_(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
	stopped_(false)
{
}

void tcp_server::start()
{
	start_accept();
	OnStart();
}

void tcp_server::stop()
{
	if (!stopped_)
	{
		stopped_ = true;

		acceptor_.cancel();
		acceptor_.close();
		OnStop();
	}
}

void tcp_server::start_accept()
{
	// Create a new connection
	GameConnection::pointer new_connection = GameConnection::create(acceptor_.get_io_service());


	// Start accepting
	acceptor_.async_accept(new_connection->socket(),
		boost::bind(&tcp_server::handle_accept, this, new_connection,
			boost::asio::placeholders::error));
}

///////////////// TCP_CONNECTION

void tcp_connection::handle_write(std::string str, const boost::system::error_code& error, size_t bytes)
{
	DoWrite(str, error, bytes);
}

void tcp_connection::handle_read(const boost::system::error_code& error, size_t bytes)
{
	std::string str;
	if (!error)
	{
		// Convert to a string
		boost::asio::streambuf::const_buffers_type bufs = m_message.data();
		str = std::string(boost::asio::buffers_begin(bufs), boost::asio::buffers_begin(bufs) + bytes);
		// "Erase" the buffer
		m_message.consume(bytes);
	}
	else
	{
		std::cout << "Error on handle read" << std::endl;
	}
	DoRead(str, error, bytes);
	if (!is_stopped) Read();

}

void tcp_connection::Stop()
{
	if (!is_stopped)
	{
		//socket_.cancel();
		socket_.close();	// this or acceptor sometimes throws exception, trying to close after it is closed
		is_stopped = true;
	}

}

boost::asio::ip::tcp::socket& tcp_connection::socket()
{
	return socket_;
}

void tcp_connection::start()
{
	is_client = false;
	is_stopped = false;
	Read();
}

void tcp_connection::start(std::string addr, std::string port)
{
	is_client = true;
	Connect(addr,port);
}



void tcp_connection::Send(std::string str)
{
	//std::vector<uint8_t> thing;
	//thing.push_back(1);
	//thing.push_back(2);
	//thing.push_back(42);
	//thing.push_back('\0');
	//thing.push_back('\n');

	//if (!is_stopped)
	//{
	//	boost::asio::async_write(
	//		socket_,
	//		boost::asio::buffer(thing.data(),thing.size()), //str.c_str(), str.size()),
	//		boost::bind(&tcp_connection::handle_write, shared_from_this(),
	//			str,
	//			boost::asio::placeholders::error,
	//			boost::asio::placeholders::bytes_transferred));
	//}
	if (!is_stopped)
	{
		boost::asio::async_write(
			socket_,
			boost::asio::buffer(str.c_str(), str.size()),
			boost::bind(&tcp_connection::handle_write, shared_from_this(),
				str,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));
	}
}

void tcp_connection::Read()
{
	// until reads until a dilimeter
	if (!is_stopped)
	{
		boost::asio::async_read_until(socket_,
			m_message,
			'\n',
			boost::bind(&tcp_connection::handle_read, shared_from_this(),
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));
	}
}

void tcp_connection::Connect(std::string addr, std::string port)
{
	// Add 	is_stopped = false; on success
	if (!is_client)
	{
		return;
	}
	endItr_ = resolver_.resolve(boost::asio::ip::tcp::resolver::query(addr, port));
	if (endItr_ != boost::asio::ip::tcp::resolver::iterator())
	{
		is_connecting = true;
		is_stopped = false;
		socket_.async_connect(endItr_->endpoint(),
			boost::bind(&tcp_connection::handle_connect, this, _1, endItr_));

	}
}

void tcp_connection::handle_connect(const boost::system::error_code &error, boost::asio::ip::tcp::resolver::iterator endItr)
{
	is_connecting = false;
	if (error)
	{
		is_stopped = true;
	}
	else
	{

	}
	Read();
	DoConnect(error, endItr);

}