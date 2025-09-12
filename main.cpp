#include <pqxx/pqxx>
#include <iostream>
#include <string>
#include "./application/application.hpp"
#include "./infrastructure/connectionPooling.hpp"
#include "./application/DTO/request-dto.hpp"
#include "./application/DTO/response-dto.hpp"
#include <regex>
#include <nlohmann/json.hpp>
#include "crow.h"

using json = nlohmann::json;

// bool phoneNumberIsValid(const string& phoneNumber)
// {
//     regex patern("^\\+[0-9]{2}-[0-9]{3}-[0-9]{3}-[0-9]{2}-[0-9]{2}$");
//     return regex_match(phoneNumber, patern);
// }

// void tempUtility(pqxx::work& tx)
// {
//     tx.exec("ALTER TABLE contacts ADD COLUMN addrress varchar(50)");
//     tx.commit();
// };
//
// void tempUtility2(pqxx::work& tx)
// {
//     tx.exec("UPDATE contacts SET addrress = 'a'");
//     tx.commit();
// }

crow::response getContacts(pqxx::nontransaction& tx, const crow::request& req)
{

    json contacts = application::getContacts(tx);

    return {contacts.dump()};
}

crow::response getCalls(pqxx::nontransaction& tx, const crow::request& req)
{
    json calls = application::getCallHistory(tx);;

    return {calls.dump()};
}

crow::response addContact(pqxx::nontransaction& tx, const crow::request& req)
{
    json request = req.body;

    json response = application::addContact(tx, request);

    return {response.dump()};
}

crow::response addCall(pqxx::nontransaction& tx, const crow::request& req)
{
    json request = req.body;
    json response = application::addCallHistory(tx, request);

    return {response.dump()};
}

crow::response editContact(pqxx::nontransaction& tx, const crow::request& req)
{

    json request = req.body;
    json response = application::editContact(tx, request);

    return {response.dump()};
}

crow::response deleteContact(pqxx::nontransaction& tx, const crow::request& req)
{
    json parsedInfo = json::parse(req.body, nullptr, false);

    json request = req.body;

    json response = application::deleteContact(tx, request);
    return {response.dump()};

}

crow::response deleteCall(pqxx::nontransaction& tx, const crow::request& req)
{

    json request = req.body;

    json response = application::deleteCallHistory(tx, request);

    return {response.dump()};

}



int main()
{
    unordered_map<string, crow::response(*)(pqxx::nontransaction& tx, const crow::request& req)> actionToFunction = {
        {"getContacts", getContacts},
        {"getCalls", getCalls},
        {"addContact", addContact},
        {"addCall", addCall},
        {"updateContact", editContact},
        {"deleteContact", deleteContact},
        {"deleteCall", deleteCall}};
    
    //initialization
    string poolString{"dbname = contactsDB user = postgres password = postgres \
      hostaddr = 127.0.0.1 port = 5432"};

    connectionPool pool(poolString, 5);

    crow::SimpleApp app;

    CROW_ROUTE(app, "/").methods("GET"_method, "POST"_method)([&pool, &actionToFunction](const crow::request& req)
    {

        auto conn = pool.acquire();
        pqxx::nontransaction tx(*conn);
        string action = req.get_header_value("action");

        if(actionToFunction.find(action) == actionToFunction.end()){
            crow::response bad(400);
            bad.write(R"({"error":"Invalid JSON. Expect {\"id\":number}."})");
            return bad;
        }
        else
        {
        crow::response res =  actionToFunction.at(req.get_header_value("action"))(tx, req);
        return res;
        }

    });

    app.port(8080).multithreaded().run();
    
}
