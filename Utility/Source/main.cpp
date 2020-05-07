#include "Utility.h"
#include "ThreadPool.h"


class A
{
public:
	A()
	{
		printf("A()\n");
	}
	A(const A&)
	{
		printf("A(const A&)\n");
	}
	A(A&&)
	{
		printf("A(A&&)\n");
	}
	A& operator=(const A&)
	{
		printf("A& operator=(const A&)\n");
		return *this;
	}
	A& operator=(const A&&)
	{
		printf("A& operator=(const A&&)\n");
		return *this;
	}
	~A()
	{
		printf("~A()\n");
	}
	void print()
	{
		printf("???");
	}
};



int main(int argc, char* argv[])
{
	try
	{
		ThreadPool tp;
		GlobalVariable<ThreadPool>::Set(&tp);

		A a3,a4;

		printf("qqqqqqqqqqqqqqqqqqqq\n");
		auto a =  AsyncTask(
			[](A a) mutable -> int
			{
				return 1;
			},
			std::move(a3)
		);
		printf("wwwwwwwwwwwwwwwwwww\n");

		Window* wnd = new Window(L"aa",800,600);
		wnd->Show();
		//wnd->Resize(1920,1080);
		Window::MsgLoop();
		delete wnd;
		return 0;
	}
	catch (const Exception& e)
	{
		MessageBoxA(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception& e)
	{
		MessageBoxA(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...)
	{
		MessageBoxA(nullptr, "No details available", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	return -1;
}



