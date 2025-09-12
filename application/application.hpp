#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "../Domain/callHistory/callHistory-service.hpp"
#include "../Domain/contact/contact-service.hpp"
#include "./DTO/request-dto.hpp"
#include "./DTO/response-dto.hpp"
#include <nlohmann/json.hpp>
#include "crow.h"
using namespace std;
using json = nlohmann::json;

namespace application
{


    static json getContacts(pqxx::nontransaction& tx)
    {
        ResponseGetContacts responseDTO = contactService::getAllContact(tx);
        json contacts;
        for (auto& contact : responseDTO.contacts)
        {
            contacts.push_back({
                {"id", contact.id}, {"name", contact.name},
                {"number", contact.number}, {"address", contact.address}
            });
        }
        return contacts;
    };



    // static vector<callHistory> getCallHistory(pqxx::work& tx)
    // {
    //     vector<callHistory>histories = callHistoryService::getAllCallHistories(tx);
    //
    //     for (int i = 0; callHistory& call : histories)
    //     {
    //         vector<int> ids = {call.callerId, call.calleeId};
    //         unordered_map<int, string> names;
    //         names[ids[0]];
    //         names[ids[1]];
    //
    //         contactService::getNamesByIds(tx, names);
    //
    //         histories[i].callerName = names[ids[0]];
    //         histories[i].calleeName = names[ids[1]];
    //         ++i;
    //     }
    //     return histories;
    // };

    inline json getCallHistory(pqxx::nontransaction& tx)
    {
        ResponseGetCallHistory responseDTO = callHistoryService::getAllCallHistories(tx);

        for (int i = 0; callHistory& call : responseDTO.callHistories)
        {
            call.otherName = contactService::getNameById(tx, call.otherContactId);
            ++i;
        }

        json calls;
        for (auto& call : responseDTO.callHistories)
        {
            calls.push_back({
                {"id", call.callId}, {"contactID", call.otherContactId},
                {"contactName", call.otherName}, {"time", call.date}
            });
        }

        return calls;

    }

    static json addContact(pqxx::nontransaction& tx, json& parsedRequest)
    {
        responseDTO::ResponseDTO response;
        if (!parsedRequest.contains("name"))
        {
            response.code = 400;
            response.body = R"({"error":"Invalid JSON. Expect {\"id\":number}."})";

        }
        string number = parsedRequest.at("number").get<std::string>();

        // if (!phoneNumberIsValid(number)) validation layer stuff
        // {
        //     response.code = 400;
        //     response.body = R"({"error":"not a valid phone number."})";
        // }

        auto requestDTO = parsedRequest.template get<addContactDTO::RequestAddContact>();
        json jresponse =  contactService::addContact(tx, requestDTO);

        return jresponse;
    };

    static json addCallHistory(pqxx::nontransaction& tx, json& request)
    {

        if (!request.contains("number"))
        {
            responseDTO::ResponseDTO response;
            response.code = 400;
            response.body = R"({"error":"Invalid JSON. Expect {\"number\":number}."})";
            return response;
        }

        auto requestDTO = request.template get<addCallDTO::RequestAddCall>();

        int otherId = contactService::getIdByNumber(tx, requestDTO.number);

        requestDTO.id = otherId;

        json jresponse = callHistoryService::addCallHistory(tx,requestDTO);
        return jresponse;

    };

    static json deleteContact(pqxx::nontransaction& tx, const json& request)
    {

        auto requestDTO = request.template get<deleteContactDTO::RequestDeleteContact>();
        callHistoryRepository::deleteCallHistoriesWithContactId(tx, requestDTO);

        json jresponse = contactService::deleteContact(tx, requestDTO);

        return jresponse;

    };

    inline crow::response deleteCallHistory(pqxx::nontransaction& tx, const RequestDeleteCall& requestDTO)
    {
        return callHistoryService::deleteCallHistory(tx, requestDTO);
    };

    static json editContact(pqxx::nontransaction& tx, const json& request)
    {
        if (!request.contains("name"))
        {
            responseDTO::ResponseDTO response;
            response.code = 400;
            response.body = R"({"error":"Invalid JSON. Expect {\"number\":number}."})";

            return response;

        }


        auto requestDTO = request.template get<editContactDTO::RequestEditContact>();

        json jresponse = contactService::editContact(tx, requestDTO);
        return jresponse;

    };


};


#endif // APPLICATION_HPP
