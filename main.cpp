#include <iostream>
#include <pqxx/pqxx>
#include "table_class.h"

using namespace std;

int main()
{
    try
    {
    pqxx::connection c(
    "host=localhost "
    "port=5432 "
    "dbname=postgres "
    "user=postgres "
    "password=password ");
    // ...
    //Table_Class TC("localhost",5432,"postgres","postgres","password");
    Table_Class TC(std::move(c));
    TC.drop_tables();
    TC.create_tables();
    TC.add_client("John","Smith","johnsmith@dmail.com");
    TC.add_client("Jane","Doe","janedoe@dmail.com");
    TC.add_client("Jane","Lee","janelee@dmail.com");
    TC.add_client("Al","Roy","alroy@dmail.com");
    TC.add_phone_num(1,"1-100-00-00");
    TC.add_phone_num(1,"1-100-00-01");
    TC.add_phone_num(2,"1-100-00-02");
    TC.add_phone_num(1,"1-100-00-03");
    TC.add_phone_num(4,"1-100-00-04");
    TC.test_query_general();
    TC.test_query_phone_nums();
    TC.update_client(3, "", "Leigh","janeleigh@dmail.com");
    TC.update_client(1,"Jon","","","1-100-00-33",3);
    TC.find_client("Jane");
    TC.find_client("","","","1-100-00-00");
    TC.remove_phone_num(1,2);
    TC.remove_client(4);
    std::cout << "\nAFTER THE CHANGES:\n";
    TC.test_query_general();
    TC.test_query_phone_nums();
    TC.drop_tables();
    TC.close_connection();

    } catch (pqxx::sql_error e)
    {
    std::cout << e.what() << std::endl;
    }
    //pqxx::work t{c};
}
