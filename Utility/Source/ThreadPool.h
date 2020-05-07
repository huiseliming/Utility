#pragma once
#include<condition_variable>
#include<thread>
#include<queue>
#include<future>
#include"Global.h"

#define AsyncTask(task,...) GlobalVariable<ThreadPool>::Get()->enqueue(std::function(task),##__VA_ARGS__);

class ThreadPool
{ 
public:
	explicit ThreadPool()
	{
		start();
	}

	~ThreadPool()
	{
		stop();
	}

	//使用传值函数参数会造成是4次移动构造，3次析构，
	//传指针和传值？
	template<typename Ret, typename ...Args>
	auto enqueue(std::function<Ret(Args...)>&& task, Args&& ...args)->std::future<Ret>
	{
		std::shared_ptr< std::packaged_task< Ret() >> wrapper = std::make_shared<std::packaged_task<Ret()>>(
			[ task = std::move(task), args = std::make_tuple(std::forward<Args>(args)...)]()->Ret
			{
				return std::apply(task, args);
			}
		);
		{
			std::unique_lock<std::mutex> UniqueLock(m_Mutex);
			m_Tasks.emplace([=] {
				(*wrapper)();
			});
		}
		m_ConditionVariable.notify_one();
		return wrapper->get_future();
	}

private:
	bool m_Stopping = false;

	std::mutex m_Mutex;
	std::vector<std::thread> m_Threads;
	std::queue<std::function<void()>> m_Tasks;
	std::condition_variable m_ConditionVariable;

	void start()
	{
		for (size_t i = 0; i < std::thread::hardware_concurrency(); i++)
		{
			m_Threads.emplace_back([=] {
				std::function<void()> task;
				while (true)
				{
					{
						std::unique_lock<std::mutex> lock(this->m_Mutex);
						m_ConditionVariable.wait(lock, [=] {return this->m_Stopping || !m_Tasks.empty(); });
						if (this->m_Stopping && m_Tasks.empty())
							break;
						task = std::move(this->m_Tasks.front());
						this->m_Tasks.pop();
					}
					task(); 
				}
			});
		}
	}

	void stop()noexcept
	{
		{
			std::unique_lock<std::mutex> lock{ m_Mutex };
			m_Stopping = true;
		}
		m_ConditionVariable.notify_all();
		for (auto& thread : m_Threads)
		{
			thread.join();
		}
	}
};