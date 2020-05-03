#include "Utility.h"

int main(int argc, char* argv[])
{
	try
	{
		Window* wnd = new Window(L"aa",800,600);
		wnd->Show();
		wnd->Resize(1920,1080);
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



