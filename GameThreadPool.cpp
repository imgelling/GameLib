#include "GameThreadPool.h"
namespace game
{
	void GameThreadPool::Stop()
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
		io_context->stop();
		worker_threads.join();
		work.reset();

	}

	void GameThreadPool::Reset()
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

		io_context.reset();
		work.reset();
	}

	bool GameThreadPool::HasStopped()
	{
		stop_mutex.lock();
		bool ret = stopped;
		stop_mutex.unlock();
		return ret;
	}

	void GameThreadPool::WorkerThread(std::shared_ptr< asio::io_context > io_context)
	{
#ifdef _DEBUG
		global_stream_lock.lock();
		std::cout << "[" << std::this_thread::get_id()
			<< "] Thread Start" << std::endl;
		global_stream_lock.unlock();
#endif
		bool running = true;
		while (running)
		{
			try
			{
				asio::error_code ec;
				io_context->run(ec);
				if (ec)
				{
					global_stream_lock.lock();
					std::cout << "[" << std::this_thread::get_id()
						<< "] Error: " << ec << std::endl;
					global_stream_lock.unlock();
				}
				break;
			}
			catch (std::exception& ex)
			{
				global_stream_lock.lock();
				std::cout << "[" << std::this_thread::get_id()
					<< "] Exception: " << ex.what() << std::endl;

				global_stream_lock.unlock();
			}

			if (stopped)
			{
				/* NEVER CALLED for some reason*/
				stop_mutex.lock();
				running = false;
				stop_mutex.unlock();
			}
		}

#ifdef _DEBUG
		global_stream_lock.lock();
		std::cout << "[" << std::this_thread::get_id()
			<< "] Thread Finish" << std::endl;
		global_stream_lock.unlock();
#endif
	}

	GameThreadPool::GameThreadPool(int NumOfThreads)
	{
		numOfThreads = NumOfThreads;
		stopped = true;
	}

	void GameThreadPool::Start()
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
			io_context = std::shared_ptr<asio::io_context>(new asio::io_context);
			work = std::shared_ptr<asio::io_context::work>(new asio::io_context::work(*io_context));
			for (int x = 0; x < numOfThreads; x++)
			{
				worker_threads.create_thread(std::bind(&GameThreadPool::WorkerThread, this, io_context));
			}

		}
		catch (std::exception& ex)
		{
			global_stream_lock.lock();
			std::cout << "Exception in " << __FUNCSIG__ << ": " << ex.what() << std::endl;
			global_stream_lock.unlock();
		}
	}
}