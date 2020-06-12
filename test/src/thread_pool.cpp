
#include <mutex>
#include <future>
#include <ThreadPool.h>
#include "GlobalVariable.h"

void main()
{
	{
		int taskNumber = 16;
		int count = 0;
		std::mutex mutex;
		std::vector<std::future<int >> vf;
		for (size_t i = 0; i < taskNumber; i++)
		{
			vf.emplace_back(GLOBAL_THREAD_POOL->AsyncPackagedTask([&] {
				for (size_t i = 0; i < 1024; i++)
				{
					std::lock_guard<std::mutex> Lock(mutex);
					count++;
				}
				return count;
			}));
		}

		for (size_t i = 0; i < vf.size(); i++)
		{
			std::cout << vf[i].get() << std::endl;
		}
	}
	int taskNumber = 16; 
	int count = 0;
	std::mutex mutex;

	for (size_t i = 0; i < taskNumber; i++)
	{
		GLOBAL_THREAD_POOL->AsyncExecuteTask([&]{
			for (size_t i = 0; i < 1024; i++)
			{
				std::lock_guard<std::mutex> Lock(mutex);
				count++;
			}
			printf("%d", count);
		});
	}
	//����ȫ���̳߳ع���������main����������֮�⣬��Ҫ�ȴ���������������߳�main������
	getchar();
	return; 
}
