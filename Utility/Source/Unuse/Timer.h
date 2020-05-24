#pragma once
#include <map>
#include <thread>
#include <mutex>
#include <chrono>
#include <atomic>
#include <list>

#include "../ThreadPool.h"
using ULONGLONG = unsigned long long;
using TimePoint = std::chrono::steady_clock::time_point;



struct Timer
{
	ULONGLONG id;
	TimePoint timepoint;
	ULONGLONG interval;
	std::function<void(void)> TimerFunc;
	ULONGLONG LoopCount = false;
	bool IsValid = false;
	bool IsAsyn = false;
};

struct TimerManager {
	bool IsRun = true;
	ULONGLONG TimeStep = 20;/*多少微秒检查一次可触发的定时器*/
	std::map<ULONGLONG,Timer *> idfindmap;
	std::map<std::chrono::steady_clock::time_point, std::list<Timer *>> waitrunning;
	std::mutex m;
	std::map<std::chrono::steady_clock::time_point, Timer> running;
	ThreadPool TimerManagerThread{ 1 };
	std::thread t{ [&]() { this->DoLoop(); } };
	void DoLoop();

	TimePoint InsertTimePoint(std::chrono::steady_clock::time_point now, Timer* timer);

	ULONGLONG AddTimer(ULONGLONG interval, std::function<void(void)>&& callback, ULONGLONG loop = 0, bool asyn = false);

	bool RemoveTimer(ULONGLONG id);

	TimerManager() = default;

	TimerManager(int timestep);

	~TimerManager();
	static TimerManager& GetTimerManager()
	{
		static TimerManager tm;
		return tm;
	}
};