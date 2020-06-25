//#include "Api_v1_InfraredCtrl.h"
//#include "Utility/Utility.h"
//#include "Service/InfraredService.h"
//using namespace Api::v1;
////add definition of your processing function here
//namespace Api
//{
//namespace v1
//{
//    void InfraredCtrl::List(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback)
//    {
//        Json::Value return_json;
//        return_json["error"] = "none";
//        return_json["statecode"] = 0;
//
//        Json::Value array_data;
//        auto device_data = GlobalVariable<InfraredService>::Get()->get_device_datas();
//        for (auto it = device_data.begin(); it != device_data.end(); it++)
//        {
//            Json::Value data;
//            data["infrared_ip"] = it->first;
//            array_data.append(data);
//        }
//        return_json["data"] = array_data;
//        
//        HttpResponsePtr resp = HttpResponse::newHttpJsonResponse(std::move(return_json));
//        callback(resp);
//    }
//
//    void InfraredCtrl::Ctrl(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, std::string json_data) const
//    {        
//        Json::Value return_json;
//        return_json["error"] = "none";
//        return_json["statecode"] = 0;
//        JSONCPP_STRING err;
//        Json::CharReaderBuilder builder;
//        Json::Value jsonPacket;
//
//        const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
//        if (!reader->parse(json_data.data(), json_data.data() + json_data.size(), &jsonPacket, &err) || !jsonPacket.isObject())
//        {
//            return_json["error"] = "bad json string!";
//            return_json["statecode"] = 1;
//        }
//        else
//        {
//            if (!jsonPacket["infrared_ip"]||
//                !jsonPacket["infrared_cmd"].isArray()
//            )
//            {
//                return_json["error"] = "Not Find infrared_ip or infrared_cmd in json";
//                return_json["statecode"] = 1;
//            }
//
//            std::string ip = jsonPacket["infrared_ip"].asString();
//            std::vector<InfraredCommand> vic;
//            for(int i = 0; i < jsonPacket["infrared_cmd"].size(); i++)
//            {
//                Json::Value& cmd = jsonPacket["infrared_cmd"][i];
//                if (cmd["index"].isInt() && cmd["execute"].isBool() && cmd["count"].isInt())
//                {
//                    InfraredCommand rc;
//                    rc.index = cmd["index"].asInt();
//                    rc.execute = cmd["execute"].asBool();
//                    rc.count = cmd["count"].asInt();
//                    vic.push_back(rc);
//                }
//            }
//
//            GlobalVariable<ThreadPool>::Get()->AsyncExecuteTask([ip = std::move(ip),vrc = std::move(vic) ] () mutable {
//                GlobalVariable<InfraredService>::Get()->pushcmd(ip, std::move(vrc));    
//            });
//        }
//        HttpResponsePtr resp = HttpResponse::newHttpJsonResponse(std::move(return_json));
//        callback(resp);
//    }
//}
//}