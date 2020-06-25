#pragma once
#include <drogon/HttpController.h>
using namespace drogon;
namespace Api
{
namespace v1
{
class InfraredCtrl:public drogon::HttpController<InfraredCtrl>
{
  public:
    METHOD_LIST_BEGIN
    METHOD_ADD(InfraredCtrl::List,"/list",Get);
    METHOD_ADD(InfraredCtrl::Ctrl,"/ctrl?data={1}",Get);

    METHOD_LIST_END
    // your declaration of processing function maybe like this:
    void List(const HttpRequestPtr& req,
              std::function<void (const HttpResponsePtr &)> &&callback);
    void Ctrl(const HttpRequestPtr& req,
                    std::function<void (const HttpResponsePtr &)> &&callback,
                    std::string json_data) const;
};
}
}

