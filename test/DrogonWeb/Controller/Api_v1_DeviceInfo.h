#pragma once
#include <drogon/HttpController.h>
#include <drogon/IOThreadStorage.h>
using namespace drogon;
namespace Api
{
namespace v1
{
  class DeviceInfo:public drogon::HttpController<DeviceInfo>
  {
  public:
    METHOD_LIST_BEGIN
    //use METHOD_ADD to add your custom processing function here;
    //path is /Api/v1/DeviceInfo/list
    METHOD_ADD(DeviceInfo::List,"/list",Get);
    //path is /Api/v1/DeviceInfo/heartbeat?data={1}
    //data is json string need remoteMac and remoteIP and remotePort and remoteName
    METHOD_ADD(DeviceInfo::Heartbeat,"/heartbeat?data={1}",Get);

    METHOD_LIST_END
    // your declaration of processing function maybe like this:
    void List(const HttpRequestPtr& req,
              std::function<void (const HttpResponsePtr &)> &&callback);
    void Heartbeat(const HttpRequestPtr& req,
                    std::function<void (const HttpResponsePtr &)> &&callback,
                    std::string json_data) const;
  };
}
}
