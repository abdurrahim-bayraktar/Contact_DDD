#ifndef CALLHISTORY_SERVICE_HPP
#define CALLHISTORY_SERVICE_HPP
#include "callHistory.hpp"
#include "callhistory-factory.hpp"
#include "callHistory-repository.hpp"
#include <vector>
using namespace std;

namespace  callHistoryService
{

    static vector<callHistory> getAllCallHistories(pqxx::work& tx)
    {
        return callHistoryRepository::getCallHistoryVector(tx);
    }

    static string addCallHistory(pqxx::work& tx, int callerId, int calleeId)
    {
        return callHistoryRepository::addCallHistory(tx, callerId, calleeId);
    };
};
#endif // CALLHISTORY_SERVICE_HPP
