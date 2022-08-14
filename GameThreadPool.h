#pragma once
// Support back to xp
#ifndef _WIN32_WINNT 
#define _WIN32_WINNT 0x0601
#endif // !_WIN32_WINNT 0x0601

#include <iostream>
#include <memory>
#include <shared_mutex>
#include <asio.hpp>

namespace game
{
	class GameThreadPool : public std::enable_shared_from_this< GameThreadPool >//boost::enable_shared_from_this< ThreadPool >

	{
	public:
		GameThreadPool(int NumOfThreads = 2);
		void Start();
		void Stop();
		bool HasStopped();
		void Reset();
		std::shared_ptr <asio::io_context> GetService() { return io_context; };

		// Used for queueing up work
		template <typename a1>
		void Enqueue(a1 arg1);
		// Will try to do the work ASAP ignoring queue
		template <typename a1>
		void Do(a1 arg1);

	private:
		void WorkerThread(std::shared_ptr< asio::io_context > io_context);
		asio::detail::thread_group worker_threads;
		bool stopped;
		int numOfThreads;
		std::shared_ptr< asio::io_context > io_context;
		std::shared_ptr< asio::io_context::work > work;
		std::shared_mutex global_stream_lock;
		std::shared_mutex stop_mutex;
	};

	template <typename a1>
	void GameThreadPool::Enqueue(a1 arg1)
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
		io_context->post(arg1);
	}

	template <typename a1>
	void GameThreadPool::Do(a1 arg1)
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
		io_context->dispatch(arg1);
	}
}