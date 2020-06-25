#pragma once
#include <drogon/HttpController.h>
#include <map>


using namespace drogon;


namespace Api
{
  namespace v1
  {
    class DeviceVideo:public drogon::HttpController<DeviceVideo>
    {
      public:
        METHOD_LIST_BEGIN
        //use METHOD_ADD to add your custom processing function here;
        METHOD_ADD(DeviceVideo::CommitVideoList,"/commitvideolist?mac={1}&data={2}",Get);//path is /Api/v1/DeviceVideo//uploadvideolist?data={1}
        METHOD_ADD(DeviceVideo::GetVideoList,"/getvideolist?mac={1}",Get);//path is /Api/v1/DeviceVideo/get
        //ADD_METHOD_TO(DeviceVideo::your_method_name,"/absolute/path/{1}/{2}/list",Get);//path is /absolute/path/{arg1}/{arg2}/list
        METHOD_LIST_END

        // your declaration of processing function maybe like this:
        void CommitVideoList(const HttpRequestPtr& 
                            req,std::function<void (const HttpResponsePtr &)> &&callback,
                            std::string mac,
                            std::string json_data);
        void GetVideoList(const HttpRequestPtr& req,
                          std::function<void (const HttpResponsePtr &)> &&callback,
                          std::string mac) const;
    };
  }
}
