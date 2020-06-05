/*
 * MIT License
 *
 * Copyright(c) 2020 DaiMingze
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this softwareand associated documentation files(the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions :
 *
 * The above copyright noticeand this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once
#include<condition_variable>
#include<thread>
#include<queue>
#include<future>
#include<iostream>
#include"Global.h"

#define GLOBAL_THREAD_POOL GlobalVariable<ThreadPool>::Get()

class ThreadPool
{ 
public:
	explicit ThreadPool()
	{
		Start();
	}
	explicit ThreadPool(uint32_t NumThread)
	{
		Start(NumThread);
	}
	~ThreadPool()
	{
		Stop();
	}

	template<typename Task, typename ...Args>
	void AsyncExecuteTask(Task&& task, Args&& ...args)
	{
		std::shared_ptr<std::function<void()>> executeTask = 
			std::make_shared<std::function<void()>>(
				[ task = std::forward<Task>(task), args = std::make_tuple(std::forward<Args>(args)...)]()
				{ return std::apply(std::move(task), std::move(args)); }
			);

		{
			std::unique_lock<std::mutex> UniqueLock(m_Mutex);
			m_Tasks.emplace([=] {
				(*executeTask)();
			});
		}
		m_ConditionVariable.notify_one();
	}

	template<typename Task>
	void AsyncExecuteTask(Task&& task)
	{
		std::shared_ptr<std::function<void()>> executeTask = std::make_shared<std::function<void()>>(std::forward<Task>(task));
		{
			std::unique_lock<std::mutex> UniqueLock(m_Mutex);
			m_Tasks.emplace([=] {
				(*executeTask)();
			});
		}
		m_ConditionVariable.notify_one();
	}


	template<typename Task, typename ...Args>
	auto AsyncPackagedTask(Task&& task, Args&& ...args)->std::future<decltype(std::forward<Task>(task)(args...))>
	{
		std::shared_ptr< std::packaged_task<decltype(std::forward<Task>(task)(args...))()> > packagedTask =
		std::make_shared< std::packaged_task<decltype(std::forward<Task>(task)(args...))()> >(	
			[task = std::forward<Task>(task), args = std::make_tuple(std::forward<Args>(args)...)]()
			{ return std::apply(std::move(task), std::move(args)); }
		);

		{
			std::unique_lock<std::mutex> UniqueLock(m_Mutex);
			m_Tasks.emplace([=] {
				(*packagedTask)();
			});
		}
		m_ConditionVariable.notify_one();
		return packagedTask->get_future();
	}

	template<typename Task>
	auto AsyncPackagedTask(Task&& task)->std::future<decltype(std::forward<Task>(task)())>
	{
		std::shared_ptr< std::packaged_task<decltype(std::forward<Task>(task)())()> > packagedTask =
		std::make_shared< std::packaged_task<decltype(std::forward<Task>(task)())()> >(task);
		{
			std::unique_lock<std::mutex> UniqueLock(m_Mutex);
			m_Tasks.emplace([=] {
				(*packagedTask)();
			});
		}
		m_ConditionVariable.notify_one();
		return packagedTask->get_future();
	}


private:
	bool m_Stopping = false;

	std::mutex m_Mutex;
	std::vector<std::thread> m_Threads;
	std::queue<std::function<void()>> m_Tasks;
	std::condition_variable m_ConditionVariable;

	void Start(uint32_t NumThread = 0)
	{
		uint32_t Count = NumThread ? NumThread : std::thread::hardware_concurrency();
		for (size_t i = 0; i < Count; i++)
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

	void Stop()noexcept
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