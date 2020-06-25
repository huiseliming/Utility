#include <iostream>
#include <drogon/drogon.h>
#include <filesystem>
#include "HTTPApp.h"

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

int main()
{
    std::cout << "Current Run Path " << std::filesystem::current_path() << std::endl;
    std::cout << "Initialize Drogon" << std::endl;
    InitializeDrogon();
    while (true)
    {
        std::cout << "[Console]";
        std::string consoleinput;
        std::cin >> consoleinput;
        if (!consoleinput.compare("exit"))
        {
            std::cout << "Application will exit" << std::endl;
            break;
        }
        else
        {
            std::cout << std::endl;
        }
    }
    std::cout << "Terminate Drogon" << std::endl;
    TerminateDrogon();
    std::cout<< "GoodBye" << std::endl;
    return 0;
}

