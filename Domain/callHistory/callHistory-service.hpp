#ifndef CALLHISTORY_SERVICE_HPP
#define CALLHISTORY_SERVICE_HPP
#include "callHistory.hpp"
#include "callhistory-factory.hpp"
#include "callHistory-repository.hpp"
#include <vector>
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

    inline string deleteCallHistory(pqxx::work& tx, int callId)
    {
        return callHistoryRepository::deleteCallHistory(tx, callId);
    }
};
#endif // CALLHISTORY_SERVICE_HPP
