#include <pqxx/pqxx>
#include <iostream>
#include <string>
#include "./application/application.hpp"
#include "./infrastructure/connectionPooling.hpp"
#include <regex>
#include <nlohmann/json.hpp>
#include "crow.h"

using json = nlohmann::json;

bool phoneNumberIsValid(const string& phoneNumber)
{
    regex patern("^\\+[0-9]{2}-[0-9]{3}-[0-9]{3}-[0-9]{2}-[0-9]{2}$");
    return regex_match(phoneNumber, patern);
}

void tempUtility(pqxx::work& tx)
{
    tx.exec("ALTER TABLE contacts ADD COLUMN addrress varchar(50)");
    tx.commit();
};

void tempUtility2(pqxx::work& tx)
{
    tx.exec("UPDATE contacts SET addrress = 'a'");
    tx.commit();
}

int main()
{

    //initialization
    string poolString{"dbname = contactsDB user = postgres password = postgres \
      hostaddr = 127.0.0.1 port = 5432"};

    connectionPool pool(poolString, 5);

    crow::SimpleApp app;

    //get contacts
    CROW_ROUTE(app, "/contacts").methods("GET"_method)([&pool](const crow::request& req)
    {
        json contacts;
        auto conn = pool.acquire();
        pqxx::work tx(*conn);
        vector<contact> contactList = contactService::getAllContact(tx);

        for (auto& contact : contactList)
        {
            contacts.push_back({ {"id", contact.id}, {"name", contact.name},
                {"number", contact.number}, {"address", contact.address} });
        }

        tx.commit();
        pool.release(conn);

        return crow::response(contacts.dump());
    });

    //get call  histories
    CROW_ROUTE(app, "/callHistory").methods("GET"_method)([&pool](const crow::request& req)
    {

        json calls;

        auto conn = pool.acquire();
        pqxx::work tx(*conn);

        auto callHistories = application::getCallHistory(tx);

        for (auto& call : callHistories)
        {
            calls.push_back({ {"id", call.callId}, {"contactID", call.otherContactId},
                {"contactName", call.otherName}, {"time", call.date} });
        }

        tx.commit();
        pool.release(conn);

        return crow::response(calls.dump());
    });

    //add a contact
    CROW_ROUTE(app, "/contacts/add").methods("POST"_method)([&pool](const crow::request& req)
    {
         auto conn = pool.acquire();
         pqxx::work tx(*conn);

         json parsedContact = json::parse(req.body, nullptr, false);
         if (!parsedContact.contains("name"))
         {
             crow::response bad(400);
             bad.add_header("Access-Control-Allow-Origin", "http://localhost:8080");
             bad.add_header("Content-Type", "application/json");
             bad.write(R"({"error":"Invalid JSON. Expect {\"id\":number}."})");
             return bad;
         }
         string number = parsedContact.at("number").get<std::string>();

         if (!phoneNumberIsValid(number))
         {
             crow::response bad(400);
             bad.add_header("Access-Control-Allow-Origin", "http://localhost:8080");
             bad.add_header("Content-Type", "application/json");
             bad.write(R"({"error":"not a valid phone number."})");
             return bad;
         }
         application::addContact(tx, parsedContact.at("name").get<std::string>(),
             number,
             parsedContact.at("address").get<std::string>());

         tx.commit();
         pool.release(conn);

         json result;
         result["result"] = "200 OK";
         crow::response response(result.dump());
         return response;
         
     });

    //add a call
    CROW_ROUTE(app, "/callHistory/add").methods("POST"_method)([&pool](const crow::request& req)
    {
        auto conn = pool.acquire();
        pqxx::work tx(*conn);

        json parsedCall = json::parse(req.body, nullptr, false);
        if (!parsedCall.contains("number"))
        {
             crow::response bad(400);
             bad.write(R"({"error":"Invalid JSON. Expect {\"number\":number}."})");
             return bad;
        }

        application::addCallHistory(tx, parsedCall.at("number").get<std::string>(),
         parsedCall.at("isIncoming").get<bool>());

        tx.commit();
        pool.release(conn);

        json result;
        result["result"] = "200 OK";
        crow::response response(result.dump());
        return response;

    });

    //update a contact
    CROW_ROUTE(app, "/contacts/<int>/update").methods("POST"_method)([&pool](const crow::request& req, int contId)
    {
        auto conn = pool.acquire();
        pqxx::work tx(*conn);

        json parsedInfo = json::parse(req.body, nullptr, false);

        if (!parsedInfo.contains("name"))
        {
             crow::response bad(400);
             bad.write(R"({"error":"Invalid JSON. Expect {\"id\":number}."})");
             return bad;
        }

        crow::response res =  application::editContact(tx, parsedInfo.at("name").get<string>(),contId);

        tx.commit();
        pool.release(conn);
        return res;
    });

    //delete a contact
    CROW_ROUTE(app, "/contacts/<int>/delete").methods("POST"_method)([&pool](const crow::request& req, int contactId)
    {
        auto conn = pool.acquire();
        pqxx::work tx(*conn);

        crow::response res = application::deleteContact(tx, contactId);
        tx.commit();
        pool.release(conn);
        return res;

    });

    //delete a call
    CROW_ROUTE(app, "/callHistory/<int>/delete").methods("POST"_method)([&pool](const crow::request& req, int callId)
    {
        auto conn = pool.acquire();
        pqxx::work tx(*conn);

        crow::response res = application::deleteCallHistory(tx, callId);

        tx.commit();
        pool.release(conn);

        return res;

    });

    app.port(8080).multithreaded().run();
    
}