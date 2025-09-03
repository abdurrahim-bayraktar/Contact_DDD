#ifndef CALLHISTORY_REPOSITORY_HPP
#define CALLHISTORY_REPOSITORY_HPP
#include "callHistory.hpp"
#include "callhistory-factory.hpp"
#include <vector>

using namespace std;

class callHistoryRepository
{
    callHistoryFactory factory_;

    public:
    callHistoryRepository();
    vector<callHistory> getCallHistoryVector(pqxx::work& tx)
    {
        pqxx::result rows = tx.exec("SELECT * FROM calls");
        vector<callHistory> callHistories;

        for (auto row : rows)
        {
            callHistories.push_back(factory_.createCallHistoryObject(row));
        }
        return callHistories;

    }

    static void populateCallHistoryNames(pqxx::work& tx, vector<callHistory>& histories)
    {

        //TODO: implement this with batch query and sets

        for (int i = 0; callHistory& call : histories)
        {
            pqxx::params callerId = call.callerId;
            pqxx::result caller = tx.exec("SELECT Name FROM contacts where ContactID = $1", callerId);
            histories[i].callerName = get<0>(caller[0].as<string>());

            pqxx::params calleeId = call.calleeId;
            pqxx::result callee = tx.exec("SELECT Name FROM contacts where ContactID = $1", calleeId);
            histories[i].calleeName = get<0>(callee[0].as<string>());
            ++i;
        }
    }
};

#endif // CALLHISTORY_REPOSITORY_HPP
