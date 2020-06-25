#pragma once
#include <drogon/HttpController.h>
using namespace drogon;
namespace Api
{
namespace v1
{
class FileCtrl:public drogon::HttpController<FileCtrl>
{
  public:
    METHOD_LIST_BEGIN
    //use METHOD_ADD to add your custom processing function here;
    METHOD_ADD(FileCtrl::list,"/list",Get);
    METHOD_ADD(FileCtrl::download,"/download?filename={1}",Get);
    METHOD_ADD(FileCtrl::upload,"/upload",Post);
    METHOD_ADD(FileCtrl::xdelete,"/xdelete",Post);
    //METHOD_ADD(FileCtrl::transferfile ,"/transferfile ",Post);
    //ADD_METHOD_TO(FileCtrl::your_method_name,"/absolute/path/{1}/{2}/list",Get);//path is /absolute/path/{arg1}/{arg2}/list
    METHOD_LIST_END

    // your declaration of processing function maybe like this:
    void list(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);
    void download(const HttpRequestPtr& req,
             std::function<void (const HttpResponsePtr &)> &&callback,
             std::string filename);
    void upload(const HttpRequestPtr& req,std::function<void (const HttpResponsePtr &)> &&callback) const;
    void xdelete(const HttpRequestPtr& req,std::function<void (const HttpResponsePtr &)> &&callback) const;
    //void transferfile(const HttpRequestPtr& req,std::function<void (const HttpResponsePtr &)> &&callback) const;
};
}
}
