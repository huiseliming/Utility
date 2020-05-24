#include <iostream>


#include <vector>
#include <thread>
#include <mutex>
#include <chrono>


#include <cassert>

inline int locktestmain()
{
	std::mutex a;
	std::vector <size_t> k;
	for (size_t i = 0; i < 100; i++)
	{
		k.push_back(0);
	}

	std::thread t1(
		[&]
			{
			auto start = std::chrono::steady_clock::now();
			for (size_t i = 0; i < 100; i++)
			{
				for (size_t j = 0; j < 10000; j++)
				{
					std::lock_guard<std::mutex> x(a);
					k[i] += j;
				}
			}
			auto end = std::chrono::steady_clock::now();
			printf("%lld\n", (end - start).count() );
		}
	);
	t1.join();
	for (size_t i = 0; i < 100; i++)
	{
		k[i] = 0;
	}
	printf("--------------------------------------------------------------\n");
	std::thread t2(
		[&]
		{
			auto start = std::chrono::steady_clock::now();
			for (size_t i = 0; i < 100; i++)
			{
				for (size_t j = 0; j < 10000; j++)
				{
					k[i] += j;
				}
			}
			auto end = std::chrono::steady_clock::now();
			printf("%lld\n", (end - start).count());
		}
	);
	t2.join();
	return 0;
}

















