#pragma once
#include <spdlog/spdlog.h>
#include <spdlog/cfg/env.h>
#include <spdlog/async.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/daily_file_sink.h>

#define SPDLOG_USE_THREAD_POOL

#define ROTATING_LOGGER_MAX_FILE_SIZE (5 * 1024 * 1024)
#include <iostream>
void Initialize_spdlog()
{
	spdlog::init_thread_pool(8192,1);
	std::vector<spdlog::sink_ptr> sinks;
	sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
	sinks.push_back(std::make_shared<spdlog::sinks::daily_file_sink_mt>("logs/daily_logger_mt.log", 23, 59));
#ifndef SPDLOG_USE_THREAD_POOL //async_logger将信息发送给线程池写入，logger直接在当前线程写入，两种都不会造成线程问题
	auto defaultLogger = std::make_shared<spdlog::logger>("default",std::begin(sinks),std::end(sinks));
#else
	auto defaultLogger = std::make_shared<spdlog::async_logger>("default",std::begin(sinks),std::end(sinks),spdlog::thread_pool());
#endif

	spdlog::register_logger(defaultLogger);
	spdlog::set_default_logger(defaultLogger);
}

void Shutdown_spdlog()
{
	//spdlog::flush_every(std::chrono::seconds(3));
	spdlog::shutdown();
}