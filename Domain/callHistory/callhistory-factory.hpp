#ifndef CALLHISTORY_FACTORY_HPP
#define CALLHISTORY_FACTORY_HPP
#include "callHistory.hpp"

using namespace std;

namespace callHistoryFactory
{
    inline callHistory createCallHistoryObject(const pqxx::row& row)
    {
        auto[callId, otherContact, date, isIncoming] = row.as<int, int, string, bool>();
        callHistory callHistory(callId, otherContact, isIncoming, date);
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

    inline pqxx::params createAddCallHistoryParams(const int& otherContact, bool isIncoming)
    {
        pqxx::params params{otherContact, isIncoming};
        return params;
    }

    inline pqxx::params createDeleteCallHistoryParams(const int& callID)
    {
        pqxx::params params{callID};
        return params;
    }


    inline pqxx::params createDeleteCallHistoriesWithContactId(const int& contactId)
    {
        pqxx::params params{contactId};
        return params;
    }

};

#endif // CALLHISTORY_FACTORY_HPP
