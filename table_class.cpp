#include "table_class.h"

Table_Class::Table_Class(pqxx::connection c) : c_(std::move(c)) {}
/*Table_Class::Table_Class(std::string host, int port, std::string dbname, std::string user, std::string password){
    c_= pqxx::connection(    "host="+host+" "
                          "port="+std::to_string(port)+" "
                          "dbname="+dbname+" "
                          "user="+user+" "
                          "password="+password+" ");
}*/
void Table_Class::create_tables(){
    pqxx::work t1{c_};
    t1.exec("CREATE TABLE IF NOT EXISTS clients.clients_general ("
           "clientID       SERIAL PRIMARY KEY,"
           "first_name     VARCHAR(50) NOT NULL,"
           "last_name      VARCHAR(50) NOT NULL,"
           "email          VARCHAR(50) NOT NULL UNIQUE"
           ");");
    t1.commit();
    pqxx::work t2{c_};
    t2.exec("CREATE TABLE IF NOT EXISTS clients.clients_phone_nums ("
            "phone_num_ID   SERIAL PRIMARY KEY,"
            "clientID       INTEGER NOT NULL REFERENCES clients.clients_general"
            " ON DELETE CASCADE"
            " ON UPDATE CASCADE,"
            "phone_num      VARCHAR(50),"
            "UNIQUE (clientID, phone_num)"
            ");");
    t2.commit();
}
void Table_Class::add_client(std::string first_name, std::string last_name, std::string email){
    pqxx::work t{c_};
    t.exec("INSERT INTO clients.clients_general VALUES (DEFAULT,'"+first_name+"','"+last_name+"','"+email+"');");
    t.commit();
}
void Table_Class::test_query_general(){
    pqxx::work t{c_};
    std::cout << "General client info:\n";
    for(auto [id,s1,s2,s3] : t.query<int,std::string,std::string,std::string>("SELECT * FROM clients.clients_general ORDER BY clientID;")){
        std::cout << std::to_string(id)+". "+s1+" "+s2+" "+s3+"\n";
    }
    t.commit();
};
void Table_Class::test_query_phone_nums(){
    pqxx::work t{c_};
    std::cout << "Telephone numbers:\n";
    for(auto [id,clid,s] : t.query<int,int,std::string>("SELECT * FROM clients.clients_phone_nums ORDER BY phone_num_ID;")){
        std::cout << std::to_string(id)+". "+std::to_string(clid)+" "+s+"\n";
    }
    t.commit();
};
void Table_Class::add_phone_num(int clientID, std::string phone_num){
    pqxx::work t{c_};
    t.exec("INSERT INTO clients.clients_phone_nums VALUES (DEFAULT,"+std::to_string(clientID)+",'"+phone_num+"');");
    t.commit();
};
void Table_Class::update_client(int clid, std::string first_name, std::string last_name, std::string email, std::string phone_num, int phone_num_index){
    std::string qstring = "";
    if(first_name != ""){
        qstring += " first_name='"+first_name+"'";
    }
    if(last_name != ""){
        if(first_name != "") qstring += ",";
        qstring += " last_name='"+last_name+"'";
    }
    if(email != ""){
        if(first_name != "" || last_name != "" ) qstring += ",";
        qstring += " email='"+email+"'";
    }
    if(qstring != ""){
        pqxx::work t1{c_};
        qstring = std::string("UPDATE clients.clients_general SET ") + qstring + " WHERE clientID=" + std::to_string(clid) + ";";
        t1.exec(qstring);
        t1.commit();
    }
    if(phone_num != ""){
        pqxx::work t2{c_};
        t2.exec("UPDATE clients.clients_phone_nums SET phone_num='"+phone_num+"' WHERE clientID=" + std::to_string(clid) + " AND"
            " phone_num_ID=(SELECT phone_num_ID FROM clients.clients_phone_nums WHERE clientID="+std::to_string(clid)+" ORDER BY phone_num_ID LIMIT 1 OFFSET "+std::to_string(phone_num_index-1)+");");
        t2.commit();
    }
};
void Table_Class::remove_phone_num(int clientID, int index){
    pqxx::work t{c_};
    if(index<=0){
        t.exec("DELETE FROM clients.clients_phone_nums WHERE clientID="+std::to_string(clientID)+";");
        t.commit();
    }else if(index>0){
        t.exec("DELETE FROM clients.clients_phone_nums WHERE phone_num_ID=("
               "SELECT phone_num_ID FROM clients.clients_phone_nums WHERE clientID="+std::to_string(clientID)+" ORDER BY phone_num_ID LIMIT 1 OFFSET "+std::to_string(index-1)+");");
        t.commit();
    }
};
void Table_Class::remove_client(int clientID){
    pqxx::work t{c_};
    t.exec("DELETE FROM clients.clients_general WHERE clientID="+std::to_string(clientID)+";");
    t.commit();
};
void Table_Class::find_client(std::string first_name, std::string last_name, std::string email, std::string phone_num){
    std::string qstring = "";
    if(first_name != "" || last_name != "" || email != ""){
        if(first_name != ""){
            qstring += " first_name='"+first_name+"'";
        }
        if(last_name != ""){
            if(first_name != "") qstring += "AND";
            qstring += " last_name='"+last_name+"'";
        }
        if(email != ""){
            if(first_name != "" || last_name != "" ) qstring += "AND";
            qstring += " email='"+email+"'";
        }
        if(qstring != ""){
            qstring = "SELECT * FROM clients.clients_general WHERE " + qstring;
        }
    }
    if(phone_num != ""){
        qstring = "SELECT * FROM clients.clients_general WHERE clientID=(SELECT clientID FROM clients.clients_phone_nums WHERE phone_num='" + phone_num + "');";
    }
    if(qstring != ""){
        pqxx::work t{c_};
        pqxx::result qres = t.exec(qstring);
        if(qres.size()==0){
            std::cout << "No matches found for your search\n";
        }else{
            std::cout << "Search results:\n";
            for(auto const &row : qres){
                for(auto const &field : row){
                    std::cout << field.c_str() << " ";
                }
                std::cout << "\n";
            }
        }
        t.commit();
    }
};
void Table_Class::drop_tables(){
    pqxx::work t1{c_};
    t1.exec("DROP TABLE IF EXISTS clients.clients_phone_nums;");
    t1.commit();
    pqxx::work t2{c_};
    t2.exec("DROP TABLE IF EXISTS clients.clients_general;");
    t2.commit();
};
void Table_Class::close_connection(){
    c_.close();
};
Table_Class::~Table_Class() = default;
