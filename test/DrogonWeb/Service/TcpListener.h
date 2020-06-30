#pragma once
#include "asio.hpp"
#include <map>
#include <iostream>
#include <array>
#include "LogDefine.h"

template<typename Session>
class TcpListener
{
public:
	TcpListener(asio::io_service& io_service, asio::ip::tcp::endpoint endpoint, std::string name = "TcpListener") :
		m_io_service(io_service),
		m_acceptor(io_service,endpoint),
		m_name(name)
	{
		DoAccept();
	}

	~TcpListener()
	{
	}

	void Stop() 
	{
		m_acceptor.close();
		for (auto it = m_sessionMap.begin(); it != m_sessionMap.end(); it++)
		{
			auto wp = it->second;
			std::weak_ptr<int > a;
			auto sp = !wp.expired() ? wp.lock() : nullptr;
			if (sp)
			{
				sp->Close();
			}
		}
	}
	
	void DebugCheckSharedRelease() 
	{
		m_acceptor.close();
		for (auto it = m_sessionMap.begin(); it != m_sessionMap.end(); it++)
		{
			auto wp = it->second;
			std::weak_ptr<int > a;
			auto sp = !wp.expired() ? wp.lock() : nullptr;
			if (sp)
			{
				spdlog::info("????????????????");
			}
		}
	}

	std::weak_ptr<Session> GetSessionBySessionName(std::string SessionName)
	{
		std::lock_guard<std::mutex> LockGuard(m_sessionLock);
		auto target = m_sessionMap.find(SessionName);
		if (target == m_sessionMap.end())
		{
			return std::weak_ptr<Session>();
		}
		return target->second;
	}

	std::weak_ptr<Session> GetSessionBegin()
	{
		std::lock_guard<std::mutex> LockGuard(m_sessionLock);
		return m_sessionMap.begin()->second;
	}

	std::vector<std::weak_ptr<Session>> GetSessions() 
	{
		std::vector<std::weak_ptr<Session>> sessions;
		std::lock_guard<std::mutex> LockGuard(m_sessionLock);
		for (auto it = m_sessionMap.begin(); it != m_sessionMap.end(); it++)
		{
			sessions.push_back(it->second);
		}
		return sessions;
	}

	template<typename DispatchHandle>
	void Dispatch(DispatchHandle&& dh)
	{
		m_io_service.dispatch(dh);
	}

private:
	void DoAccept() 
	{
		m_acceptor.async_accept(
			[this](std::error_code ec, asio::ip::tcp::socket socket)
			{
				if (!ec)
				{
					SPDLOG_INFO_FMT("<{0}:{1:d}> is connected!",
						socket.remote_endpoint().address().to_string(),
						socket.remote_endpoint().port()
					);
					std::stringstream fullAddressStream;
					std::string fullAddress;
					fullAddressStream << socket.remote_endpoint().address().to_string() << ":" << socket.remote_endpoint().port();
					fullAddressStream >> fullAddress;
					std::shared_ptr<Session> spSession = std::make_shared<Session>(std::move(socket));
					spSession->Start();
					std::lock_guard<std::mutex> LockGuard(m_sessionLock);
					m_sessionMap.insert(std::make_pair<std::string, std::weak_ptr<Session>>(std::move(fullAddress), spSession));
				}
				else
				{
					SPDLOG_INFO_FMT("{0} catch(asio::error_code)!\n error_code = {1:d}\n message = {2}", 
						m_name,
						ec.value(),
						ec.message()
					);
					return;
				}
				DoAccept();
			}
		);
	}

private:
	std::string m_name;
	asio::io_service& m_io_service;
	asio::ip::tcp::acceptor m_acceptor;
	std::map<std::string, std::weak_ptr<Session>> m_sessionMap;
	std::mutex m_sessionLock;

};

