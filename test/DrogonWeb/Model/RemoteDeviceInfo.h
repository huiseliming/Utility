/**
 *
 *  RemoteDeviceInfo.h
 *  DO NOT EDIT. This file is generated by drogon_ctl
 *
 */

#pragma once
#include <drogon/orm/Result.h>
#include <drogon/orm/Row.h>
#include <drogon/orm/Field.h>
#include <drogon/orm/SqlBinder.h>
#include <drogon/orm/Mapper.h>
#include <trantor/utils/Date.h>
#include <trantor/utils/Logger.h>
#include <json/json.h>
#include <string>
#include <memory>
#include <vector>
#include <tuple>
#include <stdint.h>
#include <iostream>

using namespace drogon::orm;
namespace drogon
{
namespace orm
{
class DbClient;
using DbClientPtr = std::shared_ptr<DbClient>;
}
}
namespace drogon_model
{
namespace ccdb 
{

class RemoteDeviceInfo
{
  public:
    struct Cols
    {
        static const std::string _id;
        static const std::string _device_name;
        static const std::string _device_ip;
        static const std::string _device_port;
        static const std::string _last_check_time;
        static const std::string _device_type;
    };

    const static int primaryKeyNumber;
    const static std::string tableName;
    const static bool hasPrimaryKey;
    const static std::string primaryKeyName;
    using PrimaryKeyType = std::string;
    const PrimaryKeyType &getPrimaryKey() const;

    /**
     * @brief constructor
     * @param r One row of records in the SQL query result.
     * @param indexOffset Set the offset to -1 to access all columns by column names, 
     * otherwise access all columns by offsets.
     * @note If the SQL is not a style of 'select * from table_name ...' (select all 
     * columns by an asterisk), please set the offset to -1.
     */
    explicit RemoteDeviceInfo(const Row &r, const ssize_t indexOffset = 0) noexcept;

    /**
     * @brief constructor
     * @param pJson The json object to construct a new instance.
     */
    explicit RemoteDeviceInfo(const Json::Value &pJson) noexcept(false);

    /**
     * @brief constructor
     * @param pJson The json object to construct a new instance.
     * @param pMasqueradingVector The aliases of table columns.
     */
    RemoteDeviceInfo(const Json::Value &pJson, const std::vector<std::string> &pMasqueradingVector) noexcept(false);

    RemoteDeviceInfo() = default;
    
    void updateByJson(const Json::Value &pJson) noexcept(false);
    void updateByMasqueradedJson(const Json::Value &pJson,
                                 const std::vector<std::string> &pMasqueradingVector) noexcept(false);
    static bool validateJsonForCreation(const Json::Value &pJson, std::string &err);
    static bool validateMasqueradedJsonForCreation(const Json::Value &,
                                                const std::vector<std::string> &pMasqueradingVector,
                                                    std::string &err);
    static bool validateJsonForUpdate(const Json::Value &pJson, std::string &err);
    static bool validateMasqueradedJsonForUpdate(const Json::Value &,
                                          const std::vector<std::string> &pMasqueradingVector,
                                          std::string &err);
    static bool validJsonOfField(size_t index,
                          const std::string &fieldName,
                          const Json::Value &pJson, 
                          std::string &err, 
                          bool isForCreation);

    /**  For column id  */
    ///Get the value of the column id, returns the default value if the column is null
    const std::string &getValueOfId() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<std::string> &getId() const noexcept;
    ///Set the value of the column id
    void setId(const std::string &pId) noexcept;
    void setId(std::string &&pId) noexcept;

    /**  For column device_name  */
    ///Get the value of the column device_name, returns the default value if the column is null
    const std::string &getValueOfDeviceName() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<std::string> &getDeviceName() const noexcept;
    ///Set the value of the column device_name
    void setDeviceName(const std::string &pDeviceName) noexcept;
    void setDeviceName(std::string &&pDeviceName) noexcept;

    /**  For column device_ip  */
    ///Get the value of the column device_ip, returns the default value if the column is null
    const std::string &getValueOfDeviceIp() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<std::string> &getDeviceIp() const noexcept;
    ///Set the value of the column device_ip
    void setDeviceIp(const std::string &pDeviceIp) noexcept;
    void setDeviceIp(std::string &&pDeviceIp) noexcept;

    /**  For column device_port  */
    ///Get the value of the column device_port, returns the default value if the column is null
    const int32_t &getValueOfDevicePort() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<int32_t> &getDevicePort() const noexcept;
    ///Set the value of the column device_port
    void setDevicePort(const int32_t &pDevicePort) noexcept;

    /**  For column last_check_time  */
    ///Get the value of the column last_check_time, returns the default value if the column is null
    const ::trantor::Date &getValueOfLastCheckTime() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<::trantor::Date> &getLastCheckTime() const noexcept;
    ///Set the value of the column last_check_time
    void setLastCheckTime(const ::trantor::Date &pLastCheckTime) noexcept;

    /**  For column device_type  */
    ///Get the value of the column device_type, returns the default value if the column is null
    const std::string &getValueOfDeviceType() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<std::string> &getDeviceType() const noexcept;
    ///Set the value of the column device_type
    void setDeviceType(const std::string &pDeviceType) noexcept;
    void setDeviceType(std::string &&pDeviceType) noexcept;
    void setDeviceTypeToNull() noexcept;


    static size_t getColumnNumber() noexcept {  return 6;  }
    static const std::string &getColumnName(size_t index) noexcept(false);

    Json::Value toJson() const;
    Json::Value toMasqueradedJson(const std::vector<std::string> &pMasqueradingVector) const;
    /// Relationship interfaces
  private:
    friend Mapper<RemoteDeviceInfo>;
    static const std::vector<std::string> &insertColumns() noexcept;
    void outputArgs(drogon::orm::internal::SqlBinder &binder) const;
    const std::vector<std::string> updateColumns() const;
    void updateArgs(drogon::orm::internal::SqlBinder &binder) const;
    ///For mysql or sqlite3
    void updateId(const uint64_t id);
    std::shared_ptr<std::string> id_;
    std::shared_ptr<std::string> deviceName_;
    std::shared_ptr<std::string> deviceIp_;
    std::shared_ptr<int32_t> devicePort_;
    std::shared_ptr<::trantor::Date> lastCheckTime_;
    std::shared_ptr<std::string> deviceType_;
    struct MetaData
    {
        const std::string colName_;
        const std::string colType_;
        const std::string colDatabaseType_;
        const ssize_t colLength_;
        const bool isAutoVal_;
        const bool isPrimaryKey_;
        const bool notNull_;
    };
    static const std::vector<MetaData> metaData_;
    bool dirtyFlag_[6]={ false };
  public:
    static const std::string &sqlForFindingByPrimaryKey()
    {
        static const std::string sql="select * from " + tableName + " where id = ?";
        return sql;                   
    }

    static const std::string &sqlForDeletingByPrimaryKey()
    {
        static const std::string sql="delete from " + tableName + " where id = ?";
        return sql;                   
    }
    std::string sqlForInserting(bool &needSelection) const
    {
        std::string sql="insert into " + tableName + " (";
        size_t parametersCount = 0;
        needSelection = false;
        if(dirtyFlag_[0])
        {
            sql += "id,";
            ++parametersCount;
        }
        if(dirtyFlag_[1])
        {
            sql += "device_name,";
            ++parametersCount;
        }
        if(dirtyFlag_[2])
        {
            sql += "device_ip,";
            ++parametersCount;
        }
        if(dirtyFlag_[3])
        {
            sql += "device_port,";
            ++parametersCount;
        }
        if(dirtyFlag_[4])
        {
            sql += "last_check_time,";
            ++parametersCount;
        }
        if(dirtyFlag_[5])
        {
            sql += "device_type,";
            ++parametersCount;
        }
        if(parametersCount > 0)
        {
            sql[sql.length()-1]=')';
            sql += " values (";
        }
        else
            sql += ") values (";
        
        if(dirtyFlag_[0])
        {
            sql.append("?,");

        } 
        if(dirtyFlag_[1])
        {
            sql.append("?,");

        } 
        if(dirtyFlag_[2])
        {
            sql.append("?,");

        } 
        if(dirtyFlag_[3])
        {
            sql.append("?,");

        } 
        if(dirtyFlag_[4])
        {
            sql.append("?,");

        } 
        if(dirtyFlag_[5])
        {
            sql.append("?,");

        } 
        if(parametersCount > 0)
        {
            sql.resize(sql.length() - 1);
        }
        sql.append(1, ')');
        LOG_TRACE << sql;
        return sql;   
    }
};
} // namespace ccdb
} // namespace drogon_model