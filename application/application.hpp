#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "../Domain/callHistory/callHistory-service.hpp"
#include "../Domain/contact/contact-service.hpp"
#include "./DTO/request-dto.hpp"
#include "./DTO/response-dto.hpp"
#include <nlohmann/json.hpp>
#include "crow.h"
using namespace std;

namespace application
{


    static vector<contact> getContacts(pqxx::work& tx)
    {
        return contactService::getAllContact(tx);
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

    inline vector<callHistory> getCallHistory(pqxx::work& tx)
    {
        vector<callHistory>histories = callHistoryService::getAllCallHistories(tx);

        for (int i = 0; callHistory& call : histories)
        {
            call.otherName = contactService::getNameById(tx, call.otherContactId);
            ++i;
        }
    return histories;

    }

    static ResponseDTO addContact(pqxx::work& tx, CrudRequestDTO dto)
    {
        return contactService::addContact(tx, dto);
    };

    static crow::response addCallHistory(pqxx::work& tx, const string& otherContactNumber, bool isIncoming)
    {
        int otherId = contactService::getIdByNumber(tx, otherContactNumber);
        CrudRequestDTO request;
        request.number = otherContactNumber;
        request.isIncoming = isIncoming;
        request.id = otherId;



        ResponseDTO response = callHistoryService::addCallHistory(tx,request);
        crow::response resp;
        resp.body = response.body;
        resp.code = response.code;
        return resp;

    };

    static crow::response deleteContact(pqxx::work& tx, const int& contactId)
    {

        callHistoryRepository::deleteCallHistoriesWithContactId(tx, contactId);
        return contactService::deleteContact(tx, contactId);
    };

    inline crow::response deleteCallHistory(pqxx::work& tx, const int& callId)
    {
        return callHistoryService::deleteCallHistory(tx, callId);
    };

    static crow::response editContact(pqxx::work& tx, const string& newName, const int& contactId)
    {
        return contactService::editContact(tx, newName, contactId);
    };


};


#endif // APPLICATION_HPP
