#pragma once
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 
#endif
#include <Windows.h>
#include <string>


class Window
{
public:
	Window();
	Window(const wchar_t* WindowName, int Width, int Height , LRESULT (CALLBACK *WndProc)(HWND, UINT, WPARAM, LPARAM) = DefaultWndProc);
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	virtual ~Window();

	void Create(const wchar_t* WindowName, int Width, int Height, LRESULT(CALLBACK* WndProc)(HWND, UINT, WPARAM, LPARAM) = DefaultWndProc);
	void Show();
	bool Resize(int Width, int Height);
	void Destroy();

	static void MsgLoop();
	static LRESULT WINAPI DefaultWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	HWND GetHWND() { return m_hWnd; }

private:
	int m_Width = 0;
	int m_Height = 0;
	HWND m_hWnd = NULL;
	WNDCLASSEX m_WndClass = {};
};
