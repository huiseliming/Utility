#pragma once
#include "Window.h"
#include <iostream>


//����̨��ӡ
namespace Console
{
    inline void Printf() {}
    inline void Print(const char* msg) { printf("%s", msg); }
    inline void Print(const wchar_t* msg) { wprintf(L"%ws", msg); }

    inline void Printf(const char* format, ...)
    {
        char buffer[256];
        va_list ap;
        va_start(ap, format);
        vsprintf_s(buffer, 256, format, ap);
        Print(buffer);
    }

    inline void Printf(const wchar_t* format, ...)
    {
        wchar_t buffer[256];
        va_list ap;
        va_start(ap, format);
        vswprintf(buffer, 256, format, ap);
        Print(buffer);
    }

    inline  std::string TranslateErrorCode(DWORD ErrorCode) noexcept
    {
        char* pMsgBuf = nullptr;
        //��ȡ��ʽ������
        const DWORD nMsgLen = FormatMessageA(
            FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr, ErrorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr
            );
        //����С��0˵��δ��ȡ����ʽ���Ĵ���
        if (nMsgLen <= 0)
            return "Unidentified error code";
        // �����ַ���
        std::string errorMsg = pMsgBuf;
        // ����ַ�����Windows��ϵͳ�ڴ棬�黹ϵͳ
        LocalFree(pMsgBuf);
        return errorMsg;
    }


#ifndef RELEASE
    inline void PrintSubMessage(const char* format, ...)
    {
        Print("--> ");
        char buffer[256];
        va_list ap;
        va_start(ap, format);
        vsprintf_s(buffer, 256, format, ap);
        Print(buffer);
        Print("\n");
    }
    inline void PrintSubMessage(const wchar_t* format, ...)
    {
        Print("--> ");
        wchar_t buffer[256];
        va_list ap;
        va_start(ap, format);
        vswprintf(buffer, 256, format, ap);
        Print(buffer);
        Print("\n");
    }
    inline void PrintSubMessage(void)
    {
    }
#endif


}

#define PRINT_LAST_ERROR() Console::Print(Console::TranslateErrorCode(GetLastError()).c_str())

#define PRINT_LAST_ERROR_IF_FALSE(x) if(!(x)) PRINT_LAST_ERROR()

#ifdef ASSERT
#undef ASSERT
#endif

#define STRINGIFY(x) #x
#define STRINGIFY_BUILTIN(x) STRINGIFY(x)
#define ASSERT(Expr,...) \
if(!(Expr)) \
{ \
    Console::Print( "[" __FILE__ " ]:[" STRINGIFY_BUILTIN(__LINE__) "] "); \
    Console::PrintSubMessage( " \'" #Expr "\' Is False " ); \
    Console::PrintSubMessage( __VA_ARGS__ ); \
    __debugbreak(); \
}


