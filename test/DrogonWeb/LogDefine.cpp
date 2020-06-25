#pragma once
#include <spdlog/spdlog.h>
#include <spdlog/cfg/env.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/daily_file_sink.h>

#define ROTATING_LOGGER_MAX_FILE_SIZE (5 * 1024 * 1024)
#include <iostream>
void Initialize_spdlog()
{
	spdlog::stdout_color_mt("console");
	spdlog::basic_logger_mt("basic_logger_mt", "logs/basic_logger_mt.log");
	spdlog::rotating_logger_mt("rotating_logger_mt", "logs/rotating_logger_mt.log", ROTATING_LOGGER_MAX_FILE_SIZE, 3);
	spdlog::daily_logger_mt("daily_logger_mt", "logs/daily_logger_mt.log", 23, 59);
}

void Shutdown_spdlog()
{
	spdlog::flush_every(std::chrono::seconds(3));
	spdlog::shutdown();
}