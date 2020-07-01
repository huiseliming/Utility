
#include <mutex>
#include <future>
#include <Utility.h>
#include <gtest/gtest.h>

GLOBAL_INITIALIZE(ThreadPool, "GlobalThreadPool")

TEST(UtilityTests, ThreadPoolTest)
{
	Utility::ConstructorGlobalObject();
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
		int v = vf[i].get();
	}
	ASSERT_EQ(count, taskNumber * 1024);
	std::shared_ptr<int> spVar;
	{
		ThreadPool tp("ThreadPool");
		std::shared_ptr<int> spCount = std::make_shared<int>(0);
		std::shared_ptr<std::mutex> spMutex = std::make_shared<std::mutex>();
		bool Set = false;
		for (size_t i = 0; i < taskNumber; i++)
		{
			tp.AsyncExecuteTask([&, spCount, spMutex]{
				for (size_t i = 0; i < 1024; i++)
				{
					std::lock_guard<std::mutex> Lock(*spMutex);
					(*spCount)++;
				}
			});
		}
		spVar = spCount;
	}
	ASSERT_EQ(*spVar, taskNumber * 1024);
	Utility::DestructorGlobalObject();
}