#ifndef CALLHISTORY_SERVICE_HPP
#define CALLHISTORY_SERVICE_HPP
#include "callHistory.hpp"
#include "callhistory-factory.hpp"
#include "callHistory-repository.hpp"
#include <vector>
#include <crow.h>
using namespace std;

namespace  callHistoryService
{

    inline vector<callHistory> getAllCallHistories(pqxx::work& tx)
    {
        return callHistoryRepository::getCallHistoryVector(tx);
    }

    inline string addCallHistory(pqxx::work& tx, int otherId, bool isIncoming)
    {
        return callHistoryRepository::addCallHistory(tx, otherId, isIncoming);
    };

    inline crow::response deleteCallHistory(pqxx::work& tx, int callId)
    {
        crow::response res;
        if (callHistoryRepository::deleteCallHistory(tx, callId) == 0)
        {
            res.code = 404;
            res.body =  "ERROR: id is not in db";
            return res;
        }
        else
        {
            res.code = 200;
            res.body =  "200 OK";
            return res;
        }
    }
};
#endif // CALLHISTORY_SERVICE_HPP
