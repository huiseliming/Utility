#include <iostream>
#include <drogon/drogon.h>
#include <filesystem>
#include <spdlog/spdlog.h>
#include "HTTPApp.h"
#include "LogDefine.h"
#include "ThreadPool.h"

//static AsioUdpService* s_pAsioUdpService = nullptr;
//static RelayService* s_pRelayService = nullptr;
//static InfraredService* s_pInfraredService = nullptr;
//
//void InitializeService()
//{
//    AsioTcpService::Initialize();
//    //Udp Service port in 50001
//    s_pAsioUdpService = new AsioUdpService(50001);
//    GlobalVariable<AsioUdpService>::Set(s_pAsioUdpService);
//    //Relay Service port in 50002
//    s_pRelayService = new RelayService(50002);
//    s_pRelayService->start();
//    GlobalVariable<RelayService>::Set(s_pRelayService);
//    //Infrared Service port in 50003
//    s_pInfraredService = new InfraredService(50003);
//    s_pInfraredService->start();
//    GlobalVariable<InfraredService>::Set(s_pInfraredService);
//}
//
//void TerminateService()
//{
//    //CleanUp
//    delete s_pAsioUdpService;
//    s_pAsioUdpService=nullptr;
//    delete s_pRelayService;
//    s_pRelayService=nullptr;
//    delete s_pInfraredService;
//    s_pInfraredService=nullptr;
//    
//    AsioTcpService::Terminate();
//}

//����ThreadPool������main֮�⹹���������
//���Խӵ������д��ڹر��¼�֮��֪ͨ���̣߳�
//�������ϵͳ�ص������ȴ����߳��˳�
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

int main()
{
    //���ô����¼��ص�֪ͨӦ�ó����˳�
    SetConsoleCtrlHandler(ConsoleCtrlCallback, TRUE);
    //��ʼ��
    Initialize_spdlog();


    std::vector<std::thread> TV;
    for (size_t i = 0; i < 100; i++)
    {
        TV.push_back(std::move(std::thread([]{
            for (size_t i = 0; i < 10000; i++)
            {
                spdlog::info("HelloHelloHelloHelloHelloHello");
            }
        })));
    }
    for (size_t i = 0; i < 100; i++)
    {
        TV[i].join();
    }

    spdlog::info("Hello");
    InitializeDrogon();
    spdlog::info("Initialize drogon");
    spdlog::info("Application Start Run");
    spdlog::info("Application StartPath->{}", std::filesystem::current_path().string());
    //��ѭ��
    while (!s_bApplicationStop)
    {
        std::this_thread::yield();
    }
    //�˳�����
    spdlog::info("Application Start Exit");
    spdlog::info("Shutdown drogon");
    ShutdownDrogon();
    spdlog::info("GoodBye");
    Shutdown_spdlog();
    return 0;
}

