#pragma once

// Support back to xp
#ifndef _WIN32_WINNT 
#define _WIN32_WINNT 0x0601
#endif // !_WIN32_WINNT 0x0601

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/bind.hpp>

#include <boost/enable_shared_from_this.hpp>
#include <boost/pointer_cast.hpp>


#include <iostream>



class ThreadPool : public boost::enable_shared_from_this< ThreadPool >

{
public:
	ThreadPool(int NumOfThreads = 2);
	void Start();
	void Stop();
	bool HasStopped();
	void Reset();
	boost::shared_ptr <boost::asio::io_service> GetService() { return io_service; };


	// Used for queueing up work
	template <typename a1>
	void Enqueue(a1 arg1);  

	// Will try to do the work ASAP ignoring queue
	template <typename a1>
	void Do(a1 arg1);


private:

	void WorkerThread(boost::shared_ptr< boost::asio::io_service > io_service);
	boost::thread_group worker_threads;
	bool stopped;
	int numOfThreads;
	boost::shared_ptr< boost::asio::io_service > io_service;
	boost::shared_ptr< boost::asio::io_service::work > work;
	boost::mutex global_stream_lock;
	boost::mutex stop_mutex;
};

template <typename a1>
void ThreadPool::Enqueue(a1 arg1)
{
	if (HasStopped())
	{
#ifdef _DEBUG
		global_stream_lock.lock();
		std::cout << "Attempted to Enqueue while service was stopped." << std::endl;
		global_stream_lock.unlock();
#endif
		return;
	}
	io_service->post(arg1);
}

template <typename a1>
void ThreadPool::Do(a1 arg1)
{
	if (HasStopped())
	{
#ifdef _DEBUG
		global_stream_lock.lock();
		std::cout << "Attempted to Do while service was stopped." << std::endl;
		global_stream_lock.unlock();
#endif
		return;
	}
	io_service->dispatch(boost::bind(arg1));
}



// Base connection
class tcp_connection : public boost::enable_shared_from_this<tcp_connection>
{

public:
	typedef boost::shared_ptr<tcp_connection> pointer;
	bool is_stopped;
	bool is_client;
	bool is_connecting;
	// Creates a new shared ptr of tcp_connection
	static pointer create(boost::asio::io_service& io_service)
	{
		return pointer(new tcp_connection(io_service));
	}

	// Returns the socket
	boost::asio::ip::tcp::socket& socket();
	// Starts up a connection for a server
	void start();
	// Starts up the connection for a client
	void start(std::string, std::string);

	// Starts an async write on this connection
	void Send(std::string str);
	// Starts an async read on this connection
	void Read();
	// Stops the connection
	void Stop();
	// Connects to server
	void Connect(std::string, std::string);


protected:
	tcp_connection(boost::asio::io_service& io_service)
		: socket_(io_service),
		resolver_(io_service)//,
							 //time_out(io_service, boost::posix_time::seconds(TIME_OUT_TIME))
	{
		is_stopped = true;
		is_connecting = false;
	}


	// Call back for asyn connect (internal)
	void handle_connect(const boost::system::error_code &error, boost::asio::ip::tcp::resolver::iterator endItr);
	virtual void DoConnect(const boost::system::error_code &error, boost::asio::ip::tcp::resolver::iterator endItr)
	{

	}


	// Call back for async read (internal)
	void handle_read(const boost::system::error_code& error, size_t bytes);
	// Called on completion of a read (external)
	virtual void DoRead(std::string str, const boost::system::error_code& error, size_t bytes)
	{

	}


	// Call back for async write (internal)
	void handle_write(std::string str, const boost::system::error_code& error, size_t bytes);


	// Called on completion of a write (external)
	virtual void DoWrite(std::string str, const boost::system::error_code& error, size_t bytes)
	{

	}


	boost::asio::ip::tcp::socket socket_;
	boost::asio::streambuf m_message;
	boost::asio::ip::tcp::resolver resolver_;
	boost::asio::ip::tcp::resolver::iterator endItr_;

};

// Child connection
class GameConnection : public tcp_connection
{
public:
	typedef boost::shared_ptr<GameConnection> pointer;
	// Creates a new shared ptr of GameConnection
	static pointer create(boost::asio::io_service& io_service)
	{
		return pointer(new GameConnection(io_service));
	}

	// Called on completion of a read, must be overrode
	void DoRead(std::string str, const boost::system::error_code& error, size_t bytes) override;

	// Called on completion of a write, must be overrode
	void DoWrite(std::string str, const boost::system::error_code& error, size_t bytes) override;

	// Called on complete of a connect, must be overrode
	void DoConnect(const boost::system::error_code &error, boost::asio::ip::tcp::resolver::iterator endItr) override;

private:
	GameConnection(boost::asio::io_service& io_service)
		: tcp_connection(io_service)
	{
	}
};

// Base server
class tcp_server
{
public:
	tcp_server(boost::asio::io_service& io_service, unsigned int port);
	// Starts the server 
	void start();
	// Stops the server
	void stop();

protected:
	// Starts accepting connections, called internally
	void start_accept();
	// Called when we accept a connection (internally)
	void handle_accept(GameConnection::pointer new_connection, const boost::system::error_code& error);
	// Called when we accept a connection (external)
	virtual void OnAccept(GameConnection::pointer new_connection, const boost::system::error_code& error) = 0;
	// Called when we start the server (external)
	virtual void OnStart() = 0;
	// Called when we stop the server (external)
	virtual void OnStop() = 0;


	boost::asio::ip::tcp::acceptor acceptor_;	// Accepts incoming connections
	bool stopped_;								// Has the server been stopped?
};

// Child server
class GameServer : public tcp_server
{
public:
	GameServer(boost::asio::io_service& io_service, unsigned int port)
		: tcp_server(io_service,port)
	{
	}
	// Called on server start, must be overrode
	void OnStart() override;
	// Called when we accept a new connection, must be overrode	
	void OnAccept(GameConnection::pointer new_connection, const boost::system::error_code& error) override;
	// Called on server stop, must be overrode	
	void OnStop() override;
};

