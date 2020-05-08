#include "Timer.h"
#include <algorithm>
void TimerManager::DoLoop()
{
	ThreadPool AsynPool(16);
	ThreadPool SyncPool(1);
	while (IsRun)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(TimeStep));
		std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();;
		{
			std::lock_guard<std::mutex> lock(m);
			decltype(waitrunning)::iterator map_it = waitrunning.begin();
			while (map_it != std::end(waitrunning) && map_it->first <= now)
			{
				decltype(map_it->second)::iterator it = std::begin(map_it->second);
				while (it != std::end(map_it->second))
				{
					if ((*it)->IsAsyn)/*异步执行*/
					{
						auto ret = AsynPool.enqueue(std::move((*it)->TimerFunc));
					}
					else                  /*同步执行*/
					{
						auto ret = SyncPool.enqueue(std::move((*it)->TimerFunc));
					}
					decltype(map_it->second)::iterator eit = it;
					it++;//先指向下一个元素
					if ((*eit)->LoopCount)
					{
						if ((*eit)->LoopCount != ULONGLONG(-1))
						{
							(*eit)->LoopCount--;
						}
						(*eit)->timepoint = (*eit)->timepoint + std::chrono::microseconds((*eit)->interval);
						InsertTimePoint((*eit)->timepoint, (*eit));
					}
					else
					{
						idfindmap.erase(idfindmap.find((*eit)->id));
						delete *eit;
					}
					map_it->second.erase(eit);
				}
				decltype(waitrunning)::iterator eit = map_it;
				map_it++;
				if (eit->second.empty())
					waitrunning.erase(eit);
			}
		}
	}
}

TimePoint TimerManager::InsertTimePoint(std::chrono::steady_clock::time_point now, Timer* timer)
{
	decltype(waitrunning)::iterator it = waitrunning.find(now);
	if (std::end(waitrunning) != it)
		it->second.emplace_back(timer);
	else 
	{
		std::list<Timer *> tl;
		tl.emplace_back(timer);
		waitrunning.insert({ now , std::move(tl) });
	}
	return now;
}
ULONGLONG TimerManager::AddTimer(ULONGLONG interval, std::function<void(void)>&& callback, ULONGLONG loop, bool asyn)
{
	static ULONGLONG idcount = 0;
	std::mutex id_m;
	ULONGLONG tid;
	{
		std::lock_guard<std::mutex> lock(id_m);
		idcount++;
		tid = idcount;
	}
	//std::function<void()> f_AddTimer= [&,tid = idcount,interval = interval, callback = std::move(callback), loop= loop, asyn = asyn]() {
	Timer * timer = new Timer();
	timer->id = tid;
	timer->timepoint = std::chrono::steady_clock::now() + std::chrono::microseconds(interval);
	timer->interval = interval;
	timer->TimerFunc = callback;
	timer->LoopCount = loop - 1;
	timer->IsAsyn = asyn;
	timer->IsValid = true;
	{
		std::lock_guard<std::mutex> lock(m);
		TimePoint tp = InsertTimePoint(timer->timepoint, timer);
		idfindmap.insert({ tid , timer });
	}
	//};
	//TimerManagerThread.enqueue(f_AddTimer);
	return tid;
}

bool TimerManager::RemoveTimer(ULONGLONG id)
{
	std::lock_guard<std::mutex> lock(m);
	decltype(idfindmap)::iterator mit = idfindmap.find(id);
	if (mit == std::end(idfindmap))
		return false;
	decltype(waitrunning)::iterator it_list_ptimer = waitrunning.find(mit->second->timepoint);
	if (std::end(waitrunning) != it_list_ptimer)
	{
		decltype(it_list_ptimer->second)::iterator it_ptimer = std::find_if(std::begin(it_list_ptimer->second), std::end(it_list_ptimer->second), [&](decltype(it_list_ptimer->second)::reference ref) { return ref->id == id; });
		if (std::end(it_list_ptimer->second) != it_ptimer)
		{
			delete *it_ptimer;
			it_list_ptimer->second.erase(it_ptimer);
			if (it_list_ptimer->second.empty())
				waitrunning.erase(it_list_ptimer);
		}
	}
	idfindmap.erase(mit);
	return true;
}

TimerManager::TimerManager(int timestep) :TimeStep(timestep)
{

}
TimerManager::~TimerManager()
{
	IsRun = false;
	t.join();
}