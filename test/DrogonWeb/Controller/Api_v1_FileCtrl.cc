#include <filesystem>
#include "Api_v1_FileCtrl.h"
#include "../Model/FileMark.h"

using namespace Api::v1;
//add definition of your processing function here
namespace Api
{
namespace v1
{
    
    void FileCtrl::list(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback)
    {
        drogon::orm::DbClientPtr db_client_ptr = drogon::app().getDbClient();
        drogon::orm::Mapper<drogon_model::ccdb::FileMark> mapper(db_client_ptr);
        std::vector<drogon_model::ccdb::FileMark> remoteDeviceInfoVector = mapper.findAll();
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
    
    void FileCtrl::download(const HttpRequestPtr& req,std::function<void (const HttpResponsePtr &)> &&callback,std::string filename)
    {
        std::cout << "Download Request :" << std::endl;
        std::string fullpath = std::filesystem::current_path().u8string() + "/file/" + filename;
        std::size_t pos = fullpath.rfind('/');
        HttpResponsePtr resp;
        if(pos != std::string::npos && std::filesystem::exists(fullpath))
        {
            std::string samplename = fullpath.substr(pos + 1, fullpath.size() - pos - 1);
            std::cout << samplename << std::endl;

            resp = HttpResponse::newFileResponse(fullpath,samplename);
            resp->addHeader("filename", drogon::utils::base64Encode((const unsigned char *)samplename.data(),samplename.size()));
        }
        else
        {
            resp = HttpResponse::newNotFoundResponse();
        }
        callback(resp);
    }

    void FileCtrl::upload(const HttpRequestPtr& req,std::function<void (const HttpResponsePtr &)> &&callback) const
    {
        Json::Value return_json;
        return_json["error"] = "none";
        return_json["statecode"] = 0;
        //Upload file check out 
        MultiPartParser fileUpload;
        if (fileUpload.parse(req) == 0)
        {
            auto& files = fileUpload.getFiles();
            for(auto const & file : files)
            {
                std::cout 
                << "filename :" << file.getFileName() << std::endl
                << "md5      :" << file.getMd5() << std::endl
                << "filelen  :" << file.fileLength() << std::endl;

                std::filesystem::current_path();
                std::string savepath = std::filesystem::current_path().u8string() + "/file/";
                if(file.save(savepath) == 0)
                {
                    drogon::orm::DbClientPtr db_client_ptr = drogon::app().getDbClient();
                    drogon::orm::Mapper<drogon_model::ccdb::FileMark> mapper(db_client_ptr);
                    Json::Value Val;
                    Val["file_name"] = file.getFileName();
                    Val["file_path"] =  savepath + file.getFileName();
                    drogon_model::ccdb::FileMark fm(Val);
                    try
                    {
                        try
                        {
                            mapper.findByPrimaryKey(fm.getPrimaryKey());
                            mapper.update(fm);
                        }
                        catch(const UnexpectedRows& e)
                        {
                            mapper.insert(fm);
                            std::cerr << e.what() << '\n';
                        }
                    }
                    catch(const DrogonDbException& e)
                    {
                        std::cerr << e.base().what() << '\n';
                        return_json["error"] = "Db Can't Update";
                        return_json["statecode"] = 1;
                    }
                }
                else
                {
                    return_json["error"] = "Can't Save to Disk";
                    return_json["statecode"] = 1;
                }
            }
        }
        else
        {
            return_json["error"] = "No File!";
            return_json["statecode"] = 1;
        }
        HttpResponsePtr resp = HttpResponse::newHttpJsonResponse(return_json);
        callback(resp);
        // std::cout << "Upload Request :" << std::endl;
        // auto atbt = req->getAttributes();
        // std::cout << req->getBody() << std::endl;
        // std::cout << req->getHeader("file") << std::endl;

        // auto ps = req->getParameters();
    }

    void FileCtrl::xdelete(const HttpRequestPtr& req,std::function<void (const HttpResponsePtr &)> &&callback) const
    {
        Json::Value return_json;
        return_json["error"] = "none";
        return_json["statecode"] = 0;
        std::cout << "Parameters BEGIN" << std::endl;
        std::string filen = req->getParameter("filename");
        std::string savepath = std::filesystem::current_path().u8string() + "/file/";
        std::string filepath = savepath + filen;
        if (std::filesystem::exists(filepath))
        {
            if(std::filesystem::remove(filepath))
            {
                drogon::orm::DbClientPtr db_client_ptr = drogon::app().getDbClient();
                drogon::orm::Mapper<drogon_model::ccdb::FileMark> mapper(db_client_ptr);
                mapper.deleteByPrimaryKey(filen);
            }
            else
            {
                return_json["error"] = "RemoveFile";
                return_json["statecode"] = 1;
            }
        }
        else
        {
            return_json["error"] = "NoFile";
            return_json["statecode"] = 1;
        }
        
        std::cout << "Parameters END" << std::endl;
        HttpResponsePtr resp = HttpResponse::newHttpJsonResponse(return_json);
        callback(resp);
    }

    // void transferfile(const HttpRequestPtr& req,std::function<void (const HttpResponsePtr &)> &&callback) const
    // {
    //     Json::Value return_json;
    //     return_json["error"] = "none";
    //     return_json["statecode"] = 0;

    //     std::string json_data = req->getParameter("data");
    //     JSONCPP_STRING err;
    //     Json::CharReaderBuilder builder;
    //     Json::Value jsonPacket;
    //     const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
    //     if (!reader->parse(json_data.data(), json_data.data() + json_data.size(), &jsonPacket, &err) || !jsonPacket.isObject())
    //     {
    //         return_json["error"] = "bad json string!";
    //         return_json["statecode"] = 1;
    //     }
    //     else
    //     {
    //         // {
    //         // "targetIP":"",
    //         // "targetPort":0,
    //         // "functionName":"",
    //         // "parameter":""
    //         // }
    //         jsonPacket["functionName"] = "RemoteCall_DownloadFile";
    //         THREAD_POOL->AsyncExecuteTask(RemoteCallFunction, 
    //             jsonPacket["targetIP"].asString(),
    //             jsonPacket["targetPort"].asString(),
    //             jsonPacket["functionName"].asString(),
    //             jsonPacket["parameter"].asString());
    //     }
    //     HttpResponsePtr resp = HttpResponse::newHttpJsonResponse(std::move(return_json));
    //     callback(resp);
    // }

}
}

