#pragma once

#include <mysql/jdbc.h>
#pragma comment(lib,"mysqlcppconn.lib")

inline void test()
{
    using namespace std;
    cout << endl;
    cout << "����ִ�� SELECT sage from student where sno = '20081001197'" << endl;

    try {
        sql::Driver* driver;
        sql::Connection* con;
        sql::Statement* stmt;
        sql::ResultSet* res;

        /* �������� */
        driver = get_driver_instance();
        con = driver->connect("tcp://192.168.1.133:3306", "daimingze", "daimingze"); // ��Ҫ�޸�Ϊ�����ݿ���û�����
        /* ���� MySQL ���ݿ� university  */
        con->setSchema("test");

        stmt = con->createStatement();
        res = stmt->executeQuery("SELECT sage from student where sno = '20081001197'"); // ��׼sql���
        while (res->next()) {
            cout << "\t MySQL replies: ";
            /* ͨ������ƫ����, 1 �����һ�� */
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








