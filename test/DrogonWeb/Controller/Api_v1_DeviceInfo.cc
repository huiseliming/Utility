#include "Api_v1_DeviceInfo.h"
#include "../Model/RemoteDeviceInfo.h"
using namespace Api::v1;
//add definition of your processing function here
namespace Api
{
namespace v1
{
    void DeviceInfo::List(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback)
    {
        drogon::orm::DbClientPtr db_client_ptr = drogon::app().getDbClient();
        drogon::orm::Mapper<drogon_model::ccdb::RemoteDeviceInfo> mapper(db_client_ptr);
        std::vector<drogon_model::ccdb::RemoteDeviceInfo> remoteDeviceInfoVector = mapper.findAll();
        Json::Value return_json;
        Json::Value dataarray;
        for (size_t i = 0; i < remoteDeviceInfoVector.size(); i++)
        {
            Json::Value data = remoteDeviceInfoVector[i].toJson();
            dataarray.append(std::move(data));
        }
        return_json["data"] = dataarray;  
        return_json["error"] = "none";
        return_json["statecode"] = 0;
        HttpResponsePtr resp = HttpResponse::newHttpJsonResponse(std::move(return_json));
        callback(resp);
    }
    void DeviceInfo::Heartbeat(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, std::string json_data) const
    {  
        drogon::orm::DbClientPtr db_client_ptr = drogon::app().getDbClient();
        drogon::orm::Mapper<drogon_model::ccdb::RemoteDeviceInfo> mapper(db_client_ptr);
        Json::Value return_json;
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
            if (!jsonPacket["remoteMac"]||
                !jsonPacket["remoteName"]||
                !jsonPacket["remoteIP"]||
                !jsonPacket["remotePort"]||
                !jsonPacket["remoteType"]
            )
            {
                return_json["error"] = "Not Find remoteMac or remoteName or remoteIP or remotePort in json";
                return_json["statecode"] = 1;
            }else
            {
                
                jsonPacket["last_check_time"] = "2012-12-12 12:12:12";
                std::cout << jsonPacket.toStyledString() << std::endl;
                std::vector<std::string> colmapper = 
                { 
                    "remoteMac", 
                    "remoteName",
                    "remoteIP",
                    "remotePort",
                    "last_check_time",
                    "remoteType"
                };
                drogon_model::ccdb::RemoteDeviceInfo updateObj(jsonPacket,colmapper);
                updateObj.setLastCheckTime(trantor::Date::now());
                mapper.update(std::move(updateObj));
                return_json["error"] = "none";
                return_json["statecode"] = 0;
            }
        }
        HttpResponsePtr resp = HttpResponse::newHttpJsonResponse(std::move(return_json));
        callback(resp);
    }

}

}