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

};

#endif // CALLHISTORY_REPOSITORY_HPP
