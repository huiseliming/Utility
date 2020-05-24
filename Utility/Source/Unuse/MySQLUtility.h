#pragma once

#include <mysql/jdbc.h>
#pragma comment(lib,"mysqlcppconn.lib")

inline void test()
{
    using namespace std;
    cout << endl;
    cout << "正在执行 SELECT sage from student where sno = '20081001197'" << endl;

    try {
        sql::Driver* driver;
        sql::Connection* con;
        sql::Statement* stmt;
        sql::ResultSet* res;

        /* 创建连接 */
        driver = get_driver_instance();
        con = driver->connect("tcp://192.168.1.133:3306", "daimingze", "daimingze"); // 需要修改为你数据库的用户密码
        /* 连接 MySQL 数据库 university  */
        con->setSchema("test");

        stmt = con->createStatement();
        res = stmt->executeQuery("SELECT sage from student where sno = '20081001197'"); // 标准sql语句
        while (res->next()) {
            cout << "\t MySQL replies: ";
            /* 通过数字偏移量, 1 代表第一列 */
            cout << res->getInt(1) << endl;
        }
        delete res;
        delete stmt;
        delete con;

    }
    catch (sql::SQLException& e) {
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
    }

    cout << endl;

    return;
}








