#pragma once
#include <drogon/HttpController.h>
using namespace drogon;
namespace Api
{
namespace v1
{
class RelayCtrl:public drogon::HttpController<RelayCtrl>
{
  public:
    METHOD_LIST_BEGIN
    //use METHOD_ADD to add your custom processing function here;
    //METHOD_ADD(RelayCtrl::get,"/{2}/{1}",Get);//path is /Api/v1/RelayCtrl/{arg2}/{arg1}
    //METHOD_ADD(RelayCtrl::your_method_name,"/{1}/{2}/list",Get);//path is /Api/v1/RelayCtrl/{arg1}/{arg2}/list
    //ADD_METHOD_TO(RelayCtrl::your_method_name,"/absolute/path/{1}/{2}/list",Get);//path is /absolute/path/{arg1}/{arg2}/list
    //METHOD_ADD(RelayCtrl::List,"/list",Get);
    //METHOD_ADD(RelayCtrl::Ctrl,"/ctrl?data={1}",Get);
    METHOD_LIST_END
    //// your declaration of processing function maybe like this:
    //void List(const HttpRequestPtr& req,
    //          std::function<void (const HttpResponsePtr &)> &&callback);
    //void Ctrl(const HttpRequestPtr& req,
    //                std::function<void (const HttpResponsePtr &)> &&callback,
    //                std::string json_data) const;
};
}
}
