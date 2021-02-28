#pragma once

// Support back to xp
#ifndef _WIN32_WINNT 
#define _WIN32_WINNT 0x0601
#endif // !_WIN32_WINNT 0x0601

#include <asio.hpp>



#include <iostream>







// Base connection
class tcp_connection : public std::enable_shared_from_this<tcp_connection>
{

public:
	typedef std::shared_ptr<tcp_connection> pointer;
	bool is_stopped;
	bool is_client;
	bool is_connecting;
	// Creates a new shared ptr of tcp_connection
	static pointer create(asio::io_context& io_context)
	{
		return pointer(new tcp_connection(io_context));
	}

	// Returns the socket
	asio::ip::tcp::socket& socket();
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
	tcp_connection(asio::io_context& io_context)
		: socket_(io_context),
		resolver_(io_context)//,
							 //time_out(io_context, boost::posix_time::seconds(TIME_OUT_TIME))
	{
		is_stopped = true;
		is_connecting = false;
		is_client = false;
	}


	// Call back for asyn connect (internal)
	void handle_connect(const asio::error_code &error, asio::ip::tcp::resolver::iterator endItr);
	virtual void DoConnect(const asio::error_code &error, asio::ip::tcp::resolver::iterator endItr)
	{

	}


	// Call back for async read (internal)
	void handle_read(const asio::error_code& error, size_t bytes);
	// Called on completion of a read (external)
	virtual void DoRead(std::string str, const asio::error_code& error, size_t bytes)
	{

	}


	// Call back for async write (internal)
	void handle_write(std::string str, const asio::error_code& error, size_t bytes);


	// Called on completion of a write (external)
	virtual void DoWrite(std::string str, const asio::error_code& error, size_t bytes)
	{

	}


	asio::ip::tcp::socket socket_;
	asio::streambuf m_message;
	asio::ip::tcp::resolver resolver_;
	asio::ip::tcp::resolver::iterator endItr_;

};

// Child connection
class GameConnection : public tcp_connection
{
public:
	typedef std::shared_ptr<GameConnection> pointer;
	// Creates a new shared ptr of GameConnection
	static pointer create(asio::io_context& io_context)
	{
		return pointer(new GameConnection(io_context));
	}

	// Called on completion of a read, must be overrode
	void DoRead(std::string str, const asio::error_code& error, size_t bytes) override;

	// Called on completion of a write, must be overrode
	void DoWrite(std::string str, const asio::error_code& error, size_t bytes) override;

	// Called on complete of a connect, must be overrode
	void DoConnect(const asio::error_code &error, asio::ip::tcp::resolver::iterator endItr) override;

private:
	GameConnection(asio::io_context& io_context)
		: tcp_connection(io_context)
	{
	}
};

// Base server
class tcp_server
{
public:
	tcp_server(asio::io_context& io_context, unsigned int port);
	// Starts the server 
	void start();
	// Stops the server
	void stop();

protected:
	// Starts accepting connections, called internally
	void start_accept();
	// Called when we accept a connection (internally)
	void handle_accept(GameConnection::pointer new_connection, const asio::error_code& error);
	// Called when we accept a connection (external)
	virtual void OnAccept(GameConnection::pointer new_connection, const asio::error_code& error) = 0;
	// Called when we start the server (external)
	virtual void OnStart() = 0;
	// Called when we stop the server (external)
	virtual void OnStop() = 0;


	asio::ip::tcp::acceptor acceptor_;	// Accepts incoming connections
	bool stopped_;								// Has the server been stopped?
	asio::io_context& io_context_;
};

// Child server
class GameServer : public tcp_server
{
public:
	GameServer(asio::io_context& io_context, unsigned int port)
		: tcp_server(io_context,port)
	{
	}
	// Called on server start, must be overrode
	void OnStart() override;
	// Called when we accept a new connection, must be overrode	
	void OnAccept(GameConnection::pointer new_connection, const asio::error_code& error) override;
	// Called on server stop, must be overrode	
	void OnStop() override;
};

