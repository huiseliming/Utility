#include "Api_v1_DeviceVideo.h"
using namespace Api::v1;
//add definition of your processing function here


  std::map<std::string, Json::Value> g_videoListMap;
  std::mutex m_videoListMutex;


namespace Api
{
namespace v1
{

    void DeviceVideo::CommitVideoList(const HttpRequestPtr& req,std::function<void (const HttpResponsePtr &)> &&callback,std::string mac,std::string json_data)
    {
        Json::Value return_json;
        return_json["error"] = "none";
        return_json["statecode"] = 0;
        JSONCPP_STRING err;
        Json::CharReaderBuilder builder;
        Json::Value jsonPacket;
        const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
        if (!reader->parse(json_data.data(), json_data.data() + json_data.size(), &jsonPacket, &err) || !jsonPacket.isObject())
        {
            return_json["error"] = "bad json string!";
            return_json["statecode"] = 1;
        }
        else
        {
            std::cout << mac << std::endl;
            std::cout << jsonPacket.toStyledString() << std::endl;
            std::cout << jsonPacket["Videos"].toStyledString() << std::endl;
            {
                std::lock_guard<std::mutex> lock(m_videoListMutex);
                g_videoListMap.insert_or_assign(mac,jsonPacket["Videos"]);
            }
        }
        HttpResponsePtr resp = HttpResponse::newHttpJsonResponse(std::move(return_json));
        callback(resp);
    }

    void DeviceVideo::GetVideoList(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, std::string mac) const
    {
        Json::Value return_json;
        return_json["error"] = "none";
        return_json["statecode"] = 0;
        Json::Value json_val;
        {
            std::lock_guard<std::mutex> lock(m_videoListMutex);
            auto it = g_videoListMap.find(mac);
            if (it != g_videoListMap.end())
            {
                json_val = it->second;
                return_json["data"] = json_val;
            }
            else
            {
                return_json["error"] = mac + "no commit VideoList!";
                return_json["statecode"] = 1;
            }
        }
        HttpResponsePtr resp = HttpResponse::newHttpJsonResponse(std::move(return_json));
        callback(resp);
    }
}
}













