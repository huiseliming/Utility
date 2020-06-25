//#include "Api_v1_RelayCtrl.h"
//#include "Utility/Utility.h"
//#include "Service/RelayService.h"
//using namespace Api::v1;
////add definition of your processing function here
//namespace Api
//{
//namespace v1
//{
//    void RelayCtrl::List(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback)
//    {
//        Json::Value return_json;
//        return_json["error"] = "none";
//        return_json["statecode"] = 0;
//
//        Json::Value array_data;
//        auto device_data = GlobalVariable<RelayService>::Get()->get_device_datas();
//        for (auto it = device_data.begin(); it != device_data.end(); it++)
//        {
//            Json::Value data;
//            data["relay_ip"] = it->first;
//            std::string state;
//            for (size_t i = 0; i < it->second.size(); i++)
//            {
//                if (it->second[i])
//                    state.append("1");
//                else
//                    state.append("0");
//            }
//            data["relay_state"] = state;
//            array_data.append(data);
//        }
//        return_json["data"] = array_data;
//        HttpResponsePtr resp = HttpResponse::newHttpJsonResponse(std::move(return_json));
//        callback(resp);
//    }
//
//    void RelayCtrl::Ctrl(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, std::string json_data) const
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
//            if (!jsonPacket["relay_ip"]||
//                !jsonPacket["relay_cmd"].isArray()
//            )
//            {
//                return_json["error"] = "Not Find relay_ip or relay_cmd in json";
//                return_json["statecode"] = 1;
//            }
//
//            std::string ip = jsonPacket["relay_ip"].asString();
//            std::vector<RelayCommand> vrc;
//            for(int i = 0; i < jsonPacket["relay_cmd"].size(); i++)
//            {
//                Json::Value& cmd = jsonPacket["relay_cmd"][i];
//                if (cmd["index"].isInt() && cmd["on"].isBool())
//                {
//                    RelayCommand rc;
//                    rc.index = cmd["index"].asInt();
//                    rc.on = cmd["on"].asBool();
//                    vrc.push_back(rc);
//                }
//            }
//
//            GlobalVariable<ThreadPool>::Get()->AsyncExecuteTask([ip = std::move(ip),vrc = std::move(vrc) ] () mutable {
//                GlobalVariable<RelayService>::Get()->pushcmd(ip, std::move(vrc));    
//            });
//        }
//        HttpResponsePtr resp = HttpResponse::newHttpJsonResponse(std::move(return_json));
//        callback(resp);
//    }
//}
//}