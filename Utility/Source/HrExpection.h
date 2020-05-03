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
		//��ȡ��ʽ������
		const DWORD nMsgLen = FormatMessageA(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
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
private:
	HRESULT m_hr;
};
