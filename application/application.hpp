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


    static ResponseGetContacts getContacts(pqxx::work& tx)
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

    inline ResponseGetCallHistory getCallHistory(pqxx::work& tx)
    {
        ResponseGetCallHistory responseDTO = callHistoryService::getAllCallHistories(tx);

        for (int i = 0; callHistory& call : responseDTO.callHistories)
        {
            call.otherName = contactService::getNameById(tx, call.otherContactId);
            ++i;
        }
    return responseDTO;

    }

    static ResponseDTO addContact(pqxx::work& tx, RequestAddContact& dto)
    {
        return contactService::addContact(tx, dto);
    };

    static ResponseDTO addCallHistory(pqxx::work& tx, RequestAddCall& requestDTO)
    {
        int otherId = contactService::getIdByNumber(tx, requestDTO.number);

        requestDTO.id = otherId;



        ResponseDTO response = callHistoryService::addCallHistory(tx,requestDTO);
        return response;

    };

    static crow::response deleteContact(pqxx::work& tx, const RequestDeleteContact& requestDTO)
    {

        callHistoryRepository::deleteCallHistoriesWithContactId(tx, requestDTO);
        return contactService::deleteContact(tx, requestDTO);
    };

    inline crow::response deleteCallHistory(pqxx::work& tx, const RequestDeleteCall& requestDTO)
    {
        return callHistoryService::deleteCallHistory(tx, requestDTO);
    };

    static ResponseDTO editContact(pqxx::work& tx, const RequestEditContact& requestDTO)
    {
        return contactService::editContact(tx, requestDTO);
    };


};


#endif // APPLICATION_HPP
