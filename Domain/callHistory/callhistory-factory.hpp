#ifndef CALLHISTORY_FACTORY_HPP
#define CALLHISTORY_FACTORY_HPP
#include "callHistory.hpp"

using namespace std;

class callHistoryFactory
{
    public:
    callHistory createCallHistoryObject(pqxx::row& row)
    {
        auto[callId, callerId, calleeId, date] = row.as<int, int , int, string>();
        callHistory callHistory(callId, callerId, calleeId, date);
        return callHistory;
    }
};

#endif // CALLHISTORY_FACTORY_HPP
