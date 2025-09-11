#ifndef CALLHISTORY_REPOSITORY_HPP
#define CALLHISTORY_REPOSITORY_HPP
#include "callHistory.hpp"
#include "callhistory-factory.hpp"
#include <vector>
#include "../../application/DTO/request-dto.hpp"
#include "../../application/DTO/response-dto.hpp"

#include "../contact/contact-factory.hpp"

using namespace std;

namespace callHistoryRepository
{
    inline ResponseGetCallHistory getCallHistoryVector(pqxx::nontransaction& tx)
    {
        const pqxx::result rows = tx.exec("SELECT callid, othercontact, time, isincoming FROM calls");
        ResponseGetCallHistory responseDTO(callHistoryFactory::createCallHistoryVector(rows));
        return responseDTO;
    }

    inline ResponseDTO addCallHistory(pqxx::nontransaction& tx, const RequestAddCall& requestDTO, const pqxx::params& params)
    {

        const pqxx::result id = tx.exec("INSERT INTO calls (otherContact, isIncoming, callee) VALUES ($1, $2, 0) RETURNING CallID", params);

        ResponseDTO response;
        response.code = 200;
        response.body = "{{'id': " + to_string(id[0][0].as<int>()) + "}}";
        return response;
    }

    inline int deleteCallHistory(pqxx::nontransaction& tx, const RequestDeleteCall& requestDTO)
    {
        const pqxx::params params{requestDTO.callId};

        const pqxx::result result = tx.exec("DELETE FROM calls WHERE CallID = $1", params);

        return result.affected_rows();

    }

    inline void deleteCallHistoriesWithContactId(pqxx::nontransaction& tx, const RequestDeleteContact& requestDTO)
    {
        const pqxx::params params{requestDTO.contactId};
        tx.exec("DELETE FROM calls WHERE othercontact = $1", params);
    }
};

#endif // CALLHISTORY_REPOSITORY_HPP
