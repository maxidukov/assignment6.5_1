#ifndef TABLE_CLASS_H
#define TABLE_CLASS_H

#include <iostream>
#include <pqxx/pqxx>
#include <string>

class Table_Class{
    pqxx::connection c_;
    public:
    Table_Class(pqxx::connection c);
    //Table_Class(std::string host, int port, std::string dbname, std::string user, std::string password); //DIDN'T WORK
    void create_tables();
    void add_client(std::string first_name, std::string last_name, std::string email);
    void add_phone_num(int clientID, std::string phone_num);
    void update_client(int clid, std::string first_name = "", std::string last_name = "", std::string email = "", std::string phone_num = "", int phone_num_index=1);
    void remove_phone_num(int clientID, int index=0);
    void remove_client(int clientID);
    void find_client(std::string first_name = "", std::string last_name = "", std::string email = "", std::string phone_num = "");
    void drop_tables();

    void test_query_general();
    void test_query_phone_nums();

    void close_connection();

    ~Table_Class();

};


#endif // TABLE_CLASS_H
