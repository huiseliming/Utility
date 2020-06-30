#include"HTTPApp.h"
#include <thread>
#include <iostream>
#include <drogon/drogon.h>
#include "LogDefine.h"
using namespace drogon;

static bool s_terminateDrogon = false;
static bool s_runDrogon = false;

static std::mutex s_drogonMutex;
static std::thread s_drogonThread;
static std::condition_variable s_drogonConditionVariable;

int DrogonMainLoop()
{
    while (!s_terminateDrogon)
    {
        {
            std::unique_lock<std::mutex> lock(s_drogonMutex);
            s_drogonConditionVariable.wait(lock, [=] {return s_terminateDrogon || s_runDrogon; });
            if (s_terminateDrogon)
                break;
            SPDLOG_INFO_FMT("Drogon Is Runing");
        }
        app().run();
        SPDLOG_INFO_FMT("Drogon Is Stop");
        std::this_thread::yield();
    }
    return 0;
}

void InitializeDrogon()
{
    s_terminateDrogon = false;
    s_runDrogon = false;
    app()
        .loadConfigFile("config.json")
        .setLogPath("./")
        .setLogLevel(trantor::Logger::kWarn)
        .addListener("0.0.0.0", 7770)
        .setThreadNum(0)
        .setClientMaxBodySize(size_t(2ull * 1024ull * 1024ull * 1024ull))
        //.registerController(std::make_shared<JsonHelloWorldCtrl>())
        .registerSyncAdvice([](const HttpRequestPtr& req) -> HttpResponsePtr {
        const auto& path = req->path();
        if (path.length() == 1 && path[0] == '/')
        {
            auto response = HttpResponse::newHttpResponse();
            response->setBody("<p>Hello, world!</p>");
            return response;
        }
        return nullptr;
    });
    s_drogonThread = std::move(std::thread(DrogonMainLoop));
    {
        std::unique_lock<std::mutex> lock(s_drogonMutex);
        s_runDrogon = true;
    }
    s_drogonConditionVariable.notify_one();
    // Wait Drogon IsRunning
    while (app().isRunning())
    {
        std::this_thread::yield();
    }
}

void ShutdownDrogon()
{
    std::unique_lock<std::mutex> lock(s_drogonMutex);
    s_runDrogon = false;
    s_terminateDrogon = true;
    app().quit();
    s_drogonThread.join();
}








