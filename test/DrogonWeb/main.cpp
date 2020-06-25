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
    std::shared_ptr<spdlog::logger> log;
    spdlog::info("Initialize spdlog");
    Initialize_spdlog();
    log = spdlog::get("basic_logger_mt");
    spdlog::info("StartPath->{}", std::filesystem::current_path().string());
    spdlog::info("Initialize drogon");
    InitializeDrogon();
    log->info("Application Start Run");
    //��ѭ��
    while (!s_bApplicationStop)
    {
        std::this_thread::yield();
    }
    //�˳�����
    log->info("Application Start Exit");
    spdlog::info("Terminate drogon");
    ShutdownDrogon();
    spdlog::info("Terminate spdlog");
    Shutdown_spdlog();
    std::cout << "GoodBye" << std::endl;
    return 0;
}

