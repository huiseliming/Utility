//#include "Api_v1_RemoteCall.h"
//#include "Utility/ThreadPool.h"
//#include "Service/NetworkLogic.h"
//using namespace Api::v1;
////add definition of your processing function here
//
//static std::map<std::string,Json::Value> s_remoteCallMappingMap;
//static std::mutex s_mappingMapMutex;
//
//
//
//namespace Api
//{
//  namespace v1
//  {
//    void RemoteCall::Execute(const HttpRequestPtr& req,std::function<void (const HttpResponsePtr &)> &&callback,std::string json_data)
//    {
//        std::cout << json_data << std::endl;
//        Json::Value return_json;
//        return_json["error"] = "none";
//        return_json["statecode"] = 0;
//        JSONCPP_STRING err;
//        Json::CharReaderBuilder builder;
//        Json::Value jsonPacket;
//        const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
//        if (!reader->parse(json_data.data(), json_data.data() + json_data.size(), &jsonPacket, &err) || !jsonPacket.isObject())
//        {
//            return_json["error"] = "bad json string!";
//            return_json["statecode"] = 1;
//        }
//        else
//        {
//            // {
//            // "targetIP":"",
//            // "targetPort":0,
//            // "functionName":"",
//            // "parameter":""
//            // }
//            THREAD_POOL->AsyncExecuteTask(RemoteCallFunction, 
//                jsonPacket["targetIP"].asString(),
//                jsonPacket["targetPort"].asString(),
//                jsonPacket["functionName"].asString(),
//                jsonPacket["parameter"].asString());
//        }
//        HttpResponsePtr resp = HttpResponse::newHttpJsonResponse(std::move(return_json));
//        callback(resp);
//    }
//    void RemoteCall::CommitFunctions(const HttpRequestPtr& req,std::function<void (const HttpResponsePtr &)> &&callback, std::string mac, std::string json_data)
//    {
//        Json::Value return_json;
//        return_json["error"] = "none";
//        return_json["statecode"] = 0;
//        JSONCPP_STRING err;
//        Json::CharReaderBuilder builder;
//        Json::Value jsonPacket;
//        const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
//        if (!reader->parse(json_data.data(), json_data.data() + json_data.size(), &jsonPacket, &err) || !jsonPacket.isObject())
//        {
//            return_json["error"] = "bad json string!";
//            return_json["statecode"] = 1;
//        }
//        else
//        {
//            {
//                std::lock_guard<std::mutex> lock(s_mappingMapMutex);
//                s_remoteCallMappingMap.insert_or_assign(mac,jsonPacket);
//            }
//        }
//        HttpResponsePtr resp = HttpResponse::newHttpJsonResponse(std::move(return_json));
//        callback(resp);
//    }
//    void RemoteCall::GetFunctions(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, std::string mac) const
//    {
//        Json::Value return_json;
//        return_json["error"] = "none";
//        return_json["statecode"] = 0;
//        Json::Value json_val;
//        {
//            std::lock_guard<std::mutex> lock(s_mappingMapMutex);
//            auto it = s_remoteCallMappingMap.find(mac);
//            if (it != s_remoteCallMappingMap.end())
//            {
//                json_val = it->second;b
//                return_json["data"] = json_val;
//            }
//            else
//            {
//                return_json["error"] = mac + "no commit VideoList!";
//                return_json["statecode"] = 1;
//            }
//        }
//        HttpResponsePtr resp = HttpResponse::newHttpJsonResponse(std::move(return_json));
//        callback(resp);
//    }
//  }
//}
//
//
