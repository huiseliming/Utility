#include "Window.h"
#include "Console.h"
#include "Global.h"

Window::Window()
{
}

Window::Window(const wchar_t* WindowName, int Width, int Height, LRESULT(CALLBACK* WndProc)(HWND, UINT, WPARAM, LPARAM))
{
	Create(WindowName, Width, Height, WndProc);
}

Window::~Window()
{
	if (m_hWnd)
		Destroy();
}

void Window::Create(const wchar_t* WindowName, int Width, int Height, LRESULT(CALLBACK* WndProc)(HWND, UINT, WPARAM, LPARAM))
{
	GlobalVariable<Window>::Set(this);
	m_Width = Width;
	m_Height = Height;
	//创建窗口类
	m_WndClass.cbSize = sizeof(WNDCLASSEX);
	m_WndClass.style = CS_CLASSDC;
	m_WndClass.lpfnWndProc = WndProc;
	m_WndClass.hInstance = GetModuleHandle(NULL);
	m_WndClass.hIcon = static_cast<HICON>(LoadImage(m_WndClass.hInstance, nullptr, IMAGE_ICON, 32, 32, 0));
	m_WndClass.lpszClassName = WindowName;
	m_WndClass.hIconSm = static_cast<HICON>(LoadImage(m_WndClass.hInstance, nullptr, IMAGE_ICON, 32, 32, 0));
	//注册窗口类
	::RegisterClassExW(&m_WndClass);
	//使用窗口类创建窗口
	m_hWnd = ::CreateWindowW(m_WndClass.lpszClassName, WindowName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, m_Width, m_Height, NULL, NULL, m_WndClass.hInstance, NULL);
}

void Window::Show()
{
	::ShowWindow(m_hWnd, SW_SHOWDEFAULT);
	PRINT_LAST_ERROR_IF_FALSE(::UpdateWindow(m_hWnd));
}

bool Window::Resize(int Width, int Height)
{
	m_Width = Width;
	m_Height = Height;
	RECT Rect;
	PRINT_LAST_ERROR_IF_FALSE(GetWindowRect(m_hWnd, &Rect));
	PRINT_LAST_ERROR_IF_FALSE(MoveWindow(m_hWnd,Rect.left, Rect.top, Width, Height,true));
	return true;
}

void Window::Destroy()
{
	//销毁窗口
	::DestroyWindow(m_hWnd);
	m_hWnd = NULL;
	::UnregisterClass(m_WndClass.lpszClassName, m_WndClass.hInstance);
}

void Window::MsgLoop()
{
	MSG Msg = {};
	while (Msg.message != WM_QUIT)
	{
		if (::PeekMessage(&Msg, NULL, 0U, 0U, PM_REMOVE))
		{
			::TranslateMessage(&Msg);
			//将消息发给窗口消息处理函数处理
			::DispatchMessage(&Msg);
			continue;
		}
	}
}

LRESULT WINAPI Window::DefaultWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_SIZE: //大小改变
		GlobalVariable<Window>::Get()->SetWidth((UINT)LOWORD(lParam));
		GlobalVariable<Window>::Get()->SetHeight((UINT)HIWORD(lParam));
		return 0;
	case WM_DESTROY: //窗口销毁
		::PostQuitMessage(0);
		return 0;
	}
	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}