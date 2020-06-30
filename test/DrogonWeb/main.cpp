#include <iostream>
#include <drogon/drogon.h>
#include <filesystem>
#include <spdlog/spdlog.h>
#include "HTTPApp.h"
#include "LogDefine.h"
#include "Utility.h"
#include "Service/TcpListener.h"
#include "Service/Session.h"

// GlobalInitialize BEGIN 
static std::function<Utility::Register<ThreadPool>()> CLASS_NAME_Global_Initializer =
[&]
{
    Utility::GetConstructorVector().emplace_back(
        []()
        {
            ThreadPool * CLASS_NAME_Global_Ptr = new ThreadPool("GlobalThreadPool");
            Utility::Global<ThreadPool>::Set(CLASS_NAME_Global_Ptr);
        }
    );
    Utility::GetDestructorVector().emplace_back(
        []()
        {
            delete Utility::Global<ThreadPool>::Get();
        }
    );
    return Utility::Register<ThreadPool>(true);
};
static Utility::Register<ThreadPool> CLASS_NAME_Register = CLASS_NAME_Global_Initializer();
//GLOBAL_INITIALIZE(ThreadPool, "GlobalThreadPool")
//GLOBAL_INITIALIZE(int)
// GlobalInitialize END


//Sevice
static asio::io_service s_io_service;
static std::thread s_io_service_thread;
static TcpListener<SessionBase>* s_pSessionBaseListener = nullptr;

void InitializeService()
{
    //SessionBase Service
    s_pSessionBaseListener = new TcpListener<SessionBase>(s_io_service, asio::ip::tcp::endpoint(asio::ip::address_v4(), 5555));
    GlobalVariable<TcpListener<SessionBase>>::Set(s_pSessionBaseListener);
    SPDLOG_INFO_FMT("RelayService Start");

    //Run io_service
    s_io_service_thread = std::move(std::thread(
        [&]
    {
        s_io_service.run();
    }
    ));
}

void ShutdownService()
{
    s_pSessionBaseListener->Stop();
    s_io_service_thread.join();
#ifndef NDEBUG
    s_pSessionBaseListener->DebugCheckSharedRelease();
#endif // DEBUG
    delete s_pSessionBaseListener;
}


#if _WIN32
//由于ThreadPool对象在main之外构造和析构，
//所以接到命令行窗口关闭事件之后通知主线程，
//阻塞这个系统回调函数等待主线程退出
static bool s_bApplicationStop = false;
static bool s_bApplicationStopComplete = true;
BOOL ConsoleCtrlCallback(DWORD CtrlType)
{
    switch (CtrlType)
    {
    case CTRL_C_EVENT:
        s_bApplicationStop = true;
        return TRUE;
    case CTRL_CLOSE_EVENT:
        s_bApplicationStop = true;
        s_bApplicationStopComplete = false;
        while(!s_bApplicationStopComplete)
            std::this_thread::yield();
        //ExitThread(0);
        return TRUE;
    default:
        return FALSE;
    }
}
#endif

int main()
{
#if _WIN32
    //设置窗口事件回调通知应用程序退出
    SetConsoleCtrlHandler(ConsoleCtrlCallback, TRUE);
#endif
    //初始化
    Initialize_spdlog();
    Utility::ConstructorGlobalObject();
    spdlog::info("Hello");
    InitializeDrogon();
    spdlog::info("Initialize drogon");
    InitializeService();
    spdlog::info("Initialize Services");
    spdlog::info("Application Start Run");
    spdlog::info("Application StartPath->{}", std::filesystem::current_path().string());
    spdlog::info("Console : input 'exit' to exit");
    //主循环
#if _WIN32
    while (!s_bApplicationStop)
#else
    while (true)
#endif
    {
        std::string consoleinput;
        std::cin >> consoleinput;
        if (!consoleinput.compare("exit"))
            break;
        std::this_thread::yield();
    }
    //退出清理
    spdlog::info("Application Start Exit");
    spdlog::info("Shutdown Services");
    ShutdownService();
    spdlog::info("Shutdown drogon");
    ShutdownDrogon();
    spdlog::info("GoodBye");
    Shutdown_spdlog();
    Utility::DestructorGlobalObject();
    system("pause");
    return 0;
}

