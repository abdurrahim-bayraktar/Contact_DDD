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

bool phoneNumberIsValid(const string& phoneNumber)
{
    regex patern("^\\+[0-9]{2}-[0-9]{3}-[0-9]{3}-[0-9]{2}-[0-9]{2}$");
    return regex_match(phoneNumber, patern);
}

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
    json contacts;

    ResponseGetContacts responseDTO = application::getContacts(tx);

    for (auto& contact : responseDTO.contacts)
    {
        contacts.push_back({
            {"id", contact.id}, {"name", contact.name},
            {"number", contact.number}, {"address", contact.address}
        });
    }


    return {contacts.dump()};
}

crow::response getCalls(pqxx::nontransaction& tx, const crow::request& req)
{
    json calls;

    ResponseGetCallHistory responseDTO = application::getCallHistory(tx);

    for (auto& call : responseDTO.callHistories)
    {
        calls.push_back({
            {"id", call.callId}, {"contactID", call.otherContactId},
            {"contactName", call.otherName}, {"time", call.date}
        });
    }



    return {calls.dump()};
}

crow::response addContact(pqxx::nontransaction& tx, const crow::request& req)
{


    json parsedContact = json::parse(req.body, nullptr, false);
    if (!parsedContact.contains("name"))
    {
        crow::response bad(400);
        bad.write(R"({"error":"Invalid JSON. Expect {\"id\":number}."})");
        return bad;
    }
    string number = parsedContact.at("number").get<std::string>();

    if (!phoneNumberIsValid(number))
    {
        crow::response bad(400);
        bad.write(R"({"error":"not a valid phone number."})");
        return bad;
    }

    RequestAddContact requestDTO(parsedContact.at("name").get<std::string>(), number, parsedContact.at("address").get<std::string>());

    ResponseDTO responseDto = application::addContact(tx, requestDTO);




    crow::response response;
    return response;
}

crow::response addCall(pqxx::nontransaction& tx, const crow::request& req)
{

    json parsedCall = json::parse(req.body, nullptr, false);
    if (!parsedCall.contains("number"))
    {
        crow::response bad(400);
        bad.write(R"({"error":"Invalid JSON. Expect {\"number\":number}."})");
        return bad;
    }

    RequestAddCall requestDTO(parsedCall.at("number").get<std::string>(), parsedCall.at("isIncoming").get<bool>());
    ResponseDTO responseDTO = application::addCallHistory(tx, requestDTO);


    crow::response response;
    response.code = responseDTO.code;
    response.body = responseDTO.body;
    return response;
}

crow::response editContact(pqxx::nontransaction& tx, const crow::request& req)
{

    json parsedInfo = json::parse(req.body, nullptr, false);

    if (!parsedInfo.contains("name"))
    {
        crow::response bad(400);
        bad.write(R"({"error":"Invalid JSON. Expect {\"id\":number}."})");
        return bad;
    }

    RequestEditContact requestDTO(parsedInfo.at("name").get<string>(), parsedInfo.at("contactId").get<int>());

    ResponseDTO responseDTO = application::editContact(tx, requestDTO);

    crow::response response;
    response.code = responseDTO.code;
    response.body = responseDTO.body;


    return response;
}

crow::response deleteContact(pqxx::nontransaction& tx, const crow::request& req)
{
    json parsedInfo = json::parse(req.body, nullptr, false);

    RequestDeleteContact requestDTO(parsedInfo.at("contactId").get<int>());

    crow::response res = application::deleteContact(tx, requestDTO);

}

crow::response deleteCall(pqxx::nontransaction& tx, const crow::request& req)
{

    json parsedInfo = json::parse(req.body, nullptr, false);
    RequestDeleteCall requestDTO(parsedInfo.at("callId").get<int>());
    crow::response res = application::deleteCallHistory(tx, requestDTO);



    return res;
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
        const pqxx::nontransaction tx(*conn);
        string action = eq.get_header_value("action");

        if(actionToFunction.find(action) == actionToFunction.end()){
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
