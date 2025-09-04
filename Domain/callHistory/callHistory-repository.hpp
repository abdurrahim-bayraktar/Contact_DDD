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
        pqxx::result rows = tx.exec("SELECT * FROM calls");
        vector<callHistory> callHistories = callHistoryFactory::createCallHistoryVector(rows);
        return callHistories;

    }

    inline string addCallHistory(pqxx::work& tx, int callerId, int calleeId)
    {
        pqxx::params params = callHistoryFactory::createAddCallHistoryParams(callerId, calleeId);
        tx.exec("INSERT INTO calls (caller, callee) VALUES ($1, $2)", params);
        tx.commit();
        return "200 OK";
    }

    inline string deleteCallHistory(pqxx::work& tx, int callID)
    {
        pqxx::params params = callHistoryFactory::createDeleteCallHistoryParams(callID);
        tx.exec("DELETE FROM calls WHERE CallID = $1", params);
        tx.commit();
        return "200 OK";

    }

    inline void deleteCallHistoriesWithContactId(pqxx::work& tx, const int& contactId)
    {
        pqxx::params params = callHistoryFactory::createDeleteCallHistoryParams(contactId);
        tx.exec("DELETE FROM calls WHERE (CallerID = $1 OR CalleeID = $1)", params);
        tx.commit();
    }

};

#endif // CALLHISTORY_REPOSITORY_HPP
