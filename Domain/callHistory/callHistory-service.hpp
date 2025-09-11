#ifndef CALLHISTORY_SERVICE_HPP
#define CALLHISTORY_SERVICE_HPP
#include <crow.h>
#include <vector>
#include "callhistory-factory.hpp"
#include "callHistory-repository.hpp"
#include "callHistory.hpp"
#include "../../application/DTO/request-dto.hpp"
#include "../../application/DTO/response-dto.hpp"
using namespace std;

namespace  callHistoryService
{

    inline ResponseGetCallHistory getAllCallHistories(pqxx::work& tx)
    {
        return callHistoryRepository::getCallHistoryVector(tx);
    }

    inline ResponseDTO addCallHistory(pqxx::work& tx, const RequestAddCall& request)
    {
        const pqxx::params params = callHistoryFactory::createAddCallHistoryParams(request);
        return callHistoryRepository::addCallHistory(tx, request, params);
    };

    inline crow::response deleteCallHistory(pqxx::work& tx, const RequestDeleteCall& requestDTO)
    {
        crow::response res;
        if (callHistoryRepository::deleteCallHistory(tx, requestDTO) == 0)
        {
            res.code = 404;
            res.body =  "ERROR: id is not in db";
            return res;
        }
        else
        {
            res.code = 200;
            res.body =  "200 OK";
            return res;
        }
    }
};
#endif // CALLHISTORY_SERVICE_HPP
