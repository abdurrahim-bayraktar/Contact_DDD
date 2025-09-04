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
        const pqxx::result rows = tx.exec("SELECT * FROM calls");
        vector<callHistory> callHistories = callHistoryFactory::createCallHistoryVector(rows);
        return callHistories;

    }

    inline string addCallHistory(pqxx::work& tx, const int& callerId, const int& calleeId)
    {
        pqxx::params params = callHistoryFactory::createAddCallHistoryParams(callerId, calleeId);
        tx.exec("INSERT INTO calls (caller, callee) VALUES ($1, $2)", params);
        tx.commit();
        return "200 OK";
    }

    inline string deleteCallHistory(pqxx::work& tx, const int& callID)
    {
        const pqxx::params params = callHistoryFactory::createDeleteCallHistoryParams(callID);
        pqxx::result result = tx.exec("DELETE FROM calls WHERE CallID = $1", params);
        if (result.affected_rows() != 0)
        {
             tx.commit();
            return "200 OK";
        }
        return "ERROR: No such ID";

    }

    inline void deleteCallHistoriesWithContactId(pqxx::work& tx, const int& contactId)
    {
        pqxx::params params = callHistoryFactory::createDeleteCallHistoryParams(contactId);
        tx.exec("DELETE FROM calls WHERE (caller = $1 OR callee = $1)", params);
        tx.commit();
    }

};

#endif // CALLHISTORY_REPOSITORY_HPP
