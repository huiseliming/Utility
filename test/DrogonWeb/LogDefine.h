#pragma once
#include <spdlog/spdlog.h>
#define STRINGIFY(x) #x
#define STRINGIFY_BUILTIN(x) STRINGIFY(x)

#define SPDLOG_TRACE_FMT(fmt,...)       spdlog::trace   (__FILE__ "(" STRINGIFY_BUILTIN(__LINE__) ") :" ##fmt,##__VA_ARGS__)
#define SPDLOG_DEBU_FMT(fmt, ...)      spdlog::debug   (__FILE__ "(" STRINGIFY_BUILTIN(__LINE__) ") :" ##fmt,##__VA_ARGS__)
#define SPDLOG_INFO_FMT(fmt, ...)       spdlog::info    (##fmt, ##__VA_ARGS__)
#define SPDLOG_WARN_FMT(fmt, ...)       spdlog::warn    (##fmt, ##__VA_ARGS__)
#define SPDLOG_ERROR_FMT(fmt, ...)      spdlog::error   (__FILE__ "(" STRINGIFY_BUILTIN(__LINE__) ") :" ##fmt,##__VA_ARGS__)
#define SPDLOG_CRITICAL_FMT(fmt, ...)   spdlog::critical(__FILE__ "(" STRINGIFY_BUILTIN(__LINE__) ") :" ##fmt,##__VA_ARGS__)

void Initialize_spdlog();

void Shutdown_spdlog();




