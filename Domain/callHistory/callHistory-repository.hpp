#ifndef CALLHISTORY_REPOSITORY_HPP
#define CALLHISTORY_REPOSITORY_HPP
#include "callHistory.hpp"
#include "callhistory-factory.hpp"
#include <vector>

using namespace std;

namespace callHistoryRepository
{

    vector<callHistory> getCallHistoryVector(pqxx::work& tx)
    {
        pqxx::result rows = tx.exec("SELECT * FROM calls");
        vector<callHistory> callHistories = callHistoryFactory::createCallHistoryVector(rows);

        for (auto row : rows)
        {
            callHistories.push_back(callHistoryFactory::createCallHistoryObject(row));
        }
        return callHistories;

    }

    static string addCallHistory(pqxx::work& tx, int callerId, int calleeId)
    {
        pqxx::params params{callerId, calleeId};
        tx.exec("INSERT INTO calls (caller, callee) VALUES ($1, $2)", params);
        tx.commit();
        return "200 OK";
    }

    static string deleteCallHistory(pqxx::work& tx, int callID)
    {
        pqxx::params params{callID};
        tx.exec("DELETE FROM calls WHERE CallID = $1", params);
        tx.commit();
        return "200 OK";

    }
    static void deleteCallHistoriesWithContactId(pqxx::work& tx, int& contactId)
    {
        pqxx::params params{contactId};
        tx.exec("DELETE FROM calls WHERE (CallerID = $1 OR CalleeID = $1)", params);
        tx.commit();
    }

};

#endif // CALLHISTORY_REPOSITORY_HPP
