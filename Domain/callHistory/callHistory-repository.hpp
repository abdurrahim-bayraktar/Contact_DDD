#ifndef CALLHISTORY_REPOSITORY_HPP
#define CALLHISTORY_REPOSITORY_HPP
#include "callHistory.hpp"
#include "callhistory-factory.hpp"
#include <vector>

#include "../contact/contact-factory.hpp"

using namespace std;

namespace callHistoryRepository
{
    inline vector<callHistory> getCallHistoryVector(pqxx::work& tx)
    {
        const pqxx::result rows = tx.exec("SELECT callid, othercontact, time, isincoming FROM calls");
        vector<callHistory> callHistories = callHistoryFactory::createCallHistoryVector(rows);
        return callHistories;
    }

    inline string addCallHistory(pqxx::work& tx, const int& otherContact, bool isIncoming)
    {
        pqxx::params params = callHistoryFactory::createAddCallHistoryParams(otherContact, isIncoming);
        tx.exec("INSERT INTO calls (otherContact, isIncoming, callee) VALUES ($1, $2, 0)", params);

        return "200 OK";
    }

    inline string deleteCallHistory(pqxx::work& tx, const int& callID)
    {
        const pqxx::params params = callHistoryFactory::createDeleteCallHistoryParams(callID);
        pqxx::result result = tx.exec("DELETE FROM calls WHERE CallID = $1", params);
        if (result.affected_rows() != 0)
        {
            return "200 OK";
        }
        return "ERROR: No such ID";
    }

    inline void deleteCallHistoriesWithContactId(pqxx::work& tx, const int& contactId)
    {
        pqxx::params params = callHistoryFactory::createDeleteCallHistoryParams(contactId);
        tx.exec("DELETE FROM calls WHERE othercontact = $1", params);
    }
};

#endif // CALLHISTORY_REPOSITORY_HPP
