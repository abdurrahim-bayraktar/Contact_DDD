#ifndef CALLHISTORY_SERVICE_HPP
#define CALLHISTORY_SERVICE_HPP
#include <crow.h>
#include <vector>
#include "callhistory-factory.hpp"
#include "callHistory-repository.hpp"
#include "callHistory.hpp"
#include "../../application/DTO/request-dto.hpp"
#include "../../application/DTO/response-dto.hpp"
using namespace std;

namespace  callHistoryService
{

    inline vector<callHistory> getAllCallHistories(pqxx::nontransaction& tx)
    {
        return callHistoryRepository::getCallHistoryVector(tx); //vector of calls
    }

    inline int addCallHistory(pqxx::nontransaction& tx, const int& id, const bool& isIncoming)
    {
        const pqxx::params params = callHistoryFactory::createAddCallHistoryParams(id, isIncoming);
        return callHistoryRepository::addCallHistory(tx, params); //id of new call
    };

    inline int deleteCallHistory(pqxx::nontransaction& tx, const int& callId)
    {
        return callHistoryRepository::deleteCallHistory(tx, callId); //number of rows changed
        // if (callHistoryRepository::deleteCallHistory(tx, callId) == 0)
        // {
        //     res.code = 404;
        //     res.body =  "ERROR: id is not in db";
        //     return res;
        // }
        // else
        // {
        //     res.code = 200;
        //     res.body =  "200 OK";
        //     return res;
        // }
    }
};
#endif // CALLHISTORY_SERVICE_HPP
