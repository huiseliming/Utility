#pragma once
#include <drogon/HttpController.h>
using namespace drogon;
namespace Api
{
  namespace v1
  {
    class RemoteCall:public drogon::HttpController<RemoteCall>
    {
      public:
        METHOD_LIST_BEGIN
        //use METHOD_ADD to add your custom processing function here;
        METHOD_ADD(RemoteCall::Execute,"/execute?data={1}",Get);//path is /Api/v1/RemoteCall//execute
        METHOD_ADD(RemoteCall::CommitFunctions,"/commitfunctions?mac={1}&data={2}",Get);//path is /Api/v1/RemoteCall//execute
        METHOD_ADD(RemoteCall::GetFunctions,"/getfunctions?mac={1}",Get);//path is /Api/v1/DeviceVideo/get
        //ADD_METHOD_TO(RemoteCall::your_method_name,"/absolute/path/{1}/{2}/list",Get);//path is /absolute/path/{arg1}/{arg2}/list



        METHOD_LIST_END
        // your declaration of processing function maybe like this:
        void Execute(const HttpRequestPtr& req,std::function<void (const HttpResponsePtr &)> &&callback,
                     std::string json_data);
        void CommitFunctions(const HttpRequestPtr& req,std::function<void (const HttpResponsePtr &)> &&callback,
              std::string mac,
              std::string json_data);
        void GetFunctions(const HttpRequestPtr& req,
            std::function<void (const HttpResponsePtr &)> &&callback,
            std::string mac) const;
    };
  }
}
