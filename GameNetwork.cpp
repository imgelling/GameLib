#include "GameNetwork.h"
#include <iostream>
#include <deque>
///////////////// THREADPOOL



///////////////// TCP_SERVER

void tcp_server::handle_accept(GameConnection::pointer new_connection, const asio::error_code& error)
{
	OnAccept(new_connection, error);

	// Start another ansyc accept
	if (!stopped_)
		start_accept();
}

tcp_server::tcp_server(asio::io_context& io_context, unsigned int port)
	: io_context_(io_context),
	acceptor_(io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)),
	stopped_(false)
{

	//this->io_context = io_context;
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
	//GameConnection::pointer new_connection = GameConnection::create(acceptor_.get_io_context());
	GameConnection::pointer new_connection = GameConnection::create(io_context_);


	// Start accepting
	acceptor_.async_accept(new_connection->socket(),
		std::bind(&tcp_server::handle_accept, this, new_connection,
			std::placeholders::_1));
}

///////////////// TCP_CONNECTION

void tcp_connection::handle_write(std::string str, const asio::error_code& error, size_t bytes)
{
	DoWrite(str, error, bytes);
}

void tcp_connection::handle_read(const asio::error_code& error, size_t bytes)
{
	std::string str;
	if (!error)
	{
		// Convert to a string
		asio::streambuf::const_buffers_type bufs = m_message.data();
		str = std::string(asio::buffers_begin(bufs), asio::buffers_begin(bufs) + bytes);
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

asio::ip::tcp::socket& tcp_connection::socket()
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
		asio::async_write(
			socket_,
			asio::buffer(str.c_str(), str.size()),
			std::bind(&tcp_connection::handle_write, shared_from_this(),
				str,
				std::placeholders::_1,
				std::placeholders::_2));// bytes_transferred));
	}
}

void tcp_connection::Read()
{
	// until reads until a dilimeter
	if (!is_stopped)
	{
		asio::async_read_until(socket_,
			m_message,
			'\n',
			std::bind(&tcp_connection::handle_read, shared_from_this(),
				std::placeholders::_1,//error,
				std::placeholders::_2));// bytes_transferred));
	}
}

void tcp_connection::Connect(std::string addr, std::string port)
{
	// Add 	is_stopped = false; on success
	if (!is_client)
	{
		return;
	}
	endItr_ = resolver_.resolve(asio::ip::tcp::resolver::query(addr, port));
	if (endItr_ != asio::ip::tcp::resolver::iterator())
	{
		is_connecting = true;
		is_stopped = false;
		socket_.async_connect(endItr_->endpoint(),
			std::bind(&tcp_connection::handle_connect, this, std::placeholders::_1, endItr_));

	}
}

void tcp_connection::handle_connect(const asio::error_code &error, asio::ip::tcp::resolver::iterator endItr)
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