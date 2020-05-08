/*
 * MIT License
 *
 * Copyright(c) 2020 DaiMingze
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this softwareand associated documentation files(the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions :
 *
 * The above copyright noticeand this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once
#include "Exception.h"
#include "Window.h"

#define HR_EXCEPT(hr) HrExpection(__LINE__,__FILE__,(hr))
#define THROW_HR_EXCEPT(hr) throw HR_EXCEPT(hr)
#define THROW_HR_EXCEPT_IF_FAILED(hr) if( FAILED((hr)) ) THROW_HR_EXCEPT(hr)

class HrExpection : public Exception
{
public:
	HrExpection(int line, const char* file, HRESULT hr) noexcept :
		Exception(line, file), m_hr(hr)
	{}
	const char* what() const noexcept
	{
		std::ostringstream oss;
		oss << GetType() << std::endl
			<< "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
			<< std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
			<< "[Description] " << GetErrorDescription() << std::endl
			<< GetOriginString();

		m_whatBuffer = oss.str();
		return m_whatBuffer.c_str();
	}
	const char* GetType() const noexcept
	{
		return "HresultExpection";
	}
	HRESULT GetErrorCode() const noexcept
	{
		return m_hr;
	}
	std::string GetErrorDescription() const noexcept
	{
		return TranslateErrorCode(m_hr);
	}
	static std::string TranslateErrorCode(HRESULT hr) noexcept
	{
		char* pMsgBuf = nullptr;
		//获取格式化错误
		const DWORD nMsgLen = FormatMessageA(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr
			);
		//长度小于0说明未获取到格式化的错误
		if (nMsgLen <= 0)
			return "Unidentified error code";
		// 拷贝字符串
		std::string errorMsg = pMsgBuf;
		// 这个字符串是Windows的系统内存，归还系统
		LocalFree(pMsgBuf);
		return errorMsg;
	}
private:
	HRESULT m_hr;
};
