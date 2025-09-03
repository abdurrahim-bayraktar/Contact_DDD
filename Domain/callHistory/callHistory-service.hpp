#ifndef CALLHISTORY_SERVICE_HPP
#define CALLHISTORY_SERVICE_HPP
#include "callHistory.hpp"
#include "callhistory-factory.hpp"
#include "callHistory-repository.hpp"
#include <vector>
using namespace std;
class callHistoryService
{
    callHistoryRepository repository_;
    callHistoryFactory factory_;

public:
    callHistoryService(callHistoryFactory factory, callHistoryRepository repository) : repository_(repository), factory_(factory){}

    vector<callHistory> getAllCallHistories(pqxx::work& tx)
    {
        return repository_.getCallHistoryVector(tx);
    }

    static string addCallHistory(pqxx::work& tx, int callerId, int calleeId)
    {
        return callHistoryRepository::addCallHistory(tx, callerId, calleeId);
    };
};
#endif // CALLHISTORY_SERVICE_HPP
