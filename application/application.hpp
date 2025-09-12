#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "../Domain/callHistory/callHistory-service.hpp"
#include "../Domain/contact/contact-service.hpp"
#include "../validation/validation.hpp"
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
        ResponseGetContacts responseDTO;
        responseDTO.contacts = contactService::getAllContact(tx);
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

    inline json getCallHistory(pqxx::nontransaction& tx)
    {
        ResponseGetCallHistory responseDTO;
        responseDTO.callHistories = callHistoryService::getAllCallHistories(tx);

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
            response.body = R"({"error":"Invalid JSON. Expect {\"number\":number}."})";
            return response;

        }
        auto requestDTO = parsedRequest.template get<addContactDTO::RequestAddContact>();
        requestDTO.number = parsedRequest.at("number").get<std::string>();

         if (!validation::phoneNumberIsValid(requestDTO.number))
         {
             response.code = 400;
             response.body = R"({"error":"not a valid phone number."})";
             return response;
         }
        response.code = 200;
        response.body = "{'code': 200, 'body': {'id': " +
            to_string(contactService::addContact(tx, requestDTO.name, requestDTO.number, requestDTO.address)) + "}}";

        json jresponse = response;

        return jresponse;
    };

    static json addCallHistory(pqxx::nontransaction& tx, json& request)
    {
        responseDTO::ResponseDTO response;
        if (!request.contains("number"))
        {
            response.code = 400;
            response.body = R"({"error":"Invalid JSON. Expect {\"number\":number}."})";
            return response;
        }

        auto requestDTO = request.template get<addCallDTO::RequestAddCall>();

        int otherId = contactService::getIdByNumber(tx, requestDTO.number);

        requestDTO.id = otherId;

        response.code = 200;
        response.body = "{'id': " +
            to_string(callHistoryService::addCallHistory(tx,requestDTO.id, requestDTO.isIncoming)) + "}";

        json jresponse = response;

        return jresponse;

    };

    static json deleteContact(pqxx::nontransaction& tx, const json& request)
    {

        responseDTO::ResponseDTO response;
        auto requestDTO = request.template get<deleteContactDTO::RequestDeleteContact>();
        callHistoryRepository::deleteCallHistoriesWithContactId(tx, requestDTO.contactId);

        response.code = 200;
        response.body = "{'id': " +
            to_string(contactService::deleteContact(tx, requestDTO.contactId)) + "}";
        json jresponse = response;

        return jresponse;

    };

    inline json deleteCallHistory(pqxx::nontransaction& tx, const json& request)
    {
        responseDTO::ResponseDTO response;
        auto requestDTO = request.template get<deleteCallDTO::RequestDeleteCall>();

        response.code = 200;
        response.body = "{'id': " +
            to_string(callHistoryService::deleteCallHistory(tx, requestDTO.callId)) + "}";
        json jresponse = response;

        return jresponse;
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

        return contactService::editContact(tx, requestDTO.name, requestDTO.id);

    };


};


#endif // APPLICATION_HPP
