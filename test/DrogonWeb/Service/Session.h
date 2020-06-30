#pragma once
#include "asio.hpp"
#include "LogDefine.h"
#define MAX_DATABUF_SIZE 10240


#define SESSION_TRY()  try \
{

#define SESSION_CATCH(SessionName) \
}\
catch (const asio::error_code ec)\
{\
	SPDLOG_ERROR_FMT("Session <{0}> : catch(asio::error_code)\n error_code = {1:d}\n message = {2}",\
		(SessionName),\
		ec.value(),\
		ec.message());\
	Close();\
}\
catch (const std::exception& e)\
{\
	SPDLOG_ERROR_FMT("Session <{0}> : catch(std::exception) what = {1}",\
		(SessionName),\
		e.what());\
	Close();\
}\
catch (...)\
{\
	SPDLOG_ERROR_FMT("Session <{0}> : catch(Unknow Exception)",(SessionName));\
	Close();\
}


class SessionBase : public std::enable_shared_from_this<SessionBase>
{
public:
	SessionBase(asio::ip::tcp::socket socket):
		m_socket(std::move(socket))
	{
		m_ip = m_socket.remote_endpoint().address().to_string();
		m_port = m_socket.remote_endpoint().port();
	}
	~SessionBase()
	{
		SPDLOG_INFO_FMT("<{0}:{1:d}> is disconnected!",
			m_ip,
			m_port
		);
	}

	virtual void Start()
	{
		DoRead();
	}

	void Close()
	{
		m_socket.close();
	}

protected:

	virtual void DoRead()
	{
		auto self(shared_from_this());
		m_socket.async_read_some(
			asio::buffer(m_dataBuf, MAX_DATABUF_SIZE),
			[this, self](std::error_code ec, std::size_t length)
			{
				if (!ec)
				{
					std::string binFmt((const char const *)&m_dataBuf[0], length);
					SPDLOG_INFO_FMT("<{0}:{1}> Recv : {2}",
						m_socket.remote_endpoint().address().to_string(),
						m_socket.remote_endpoint().port(),
						binFmt
						);
					std::vector<uint8_t> buffer;
					buffer.resize(length);
					std::memcpy(buffer.data(), &m_dataBuf[0], length);
					uint8_t* pdata = buffer.data();
					DoRead();
				}
				else
				{
					SPDLOG_ERROR_FMT("session <{0}:{1}> : error_code = {2:d}, message = {3}",
						m_ip,
						m_port,
						ec.value(),
						ec.message());
					m_socket.close();
				}
			}
		);
	}

	std::string m_ip;
	uint32_t m_port;
	asio::ip::tcp::socket m_socket;
	std::array<char, MAX_DATABUF_SIZE> m_dataBuf;
};


















