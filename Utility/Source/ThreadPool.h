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