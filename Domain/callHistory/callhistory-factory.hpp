#ifndef CALLHISTORY_FACTORY_HPP
#define CALLHISTORY_FACTORY_HPP
#include "callHistory.hpp"

using namespace std;

namespace callHistoryFactory
{
    inline callHistory createCallHistoryObject(const pqxx::row& row)
    {
        auto[callId, callerId, calleeId, date] = row.as<int, int , int, string>();
        callHistory callHistory(callId, callerId, calleeId, date);
        return callHistory;
    }

    inline vector<callHistory> createCallHistoryVector(const pqxx::result& rows)
    {
        vector<callHistory> callHistories;
        for (auto row : rows)
        {
            callHistories.push_back(callHistoryFactory::createCallHistoryObject(row));
        }
        return callHistories;
    }
};

#endif // CALLHISTORY_FACTORY_HPP
