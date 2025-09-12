//
// Created by user on 9/10/25.
//

#ifndef CONTACT_DDD_RESPONSE_DTO_HPP
#define CONTACT_DDD_RESPONSE_DTO_HPP
#include <string>
#include <vector>
#include "../../Domain/callHistory/callHistory.hpp"
#include "../../Domain/contact/contact.hpp"
using namespace std;

namespace responseDTO
{
    class ResponseDTO
    {
    public:
        int code;
        string body;
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(ResponseDTO, code, body)
    };
}

    struct ResponseGetContacts
    {
        ResponseGetContacts()= default;
        explicit ResponseGetContacts(const vector<contact> &contacts): contacts(contacts) {}
        int code{};
        vector<contact> contacts;
    };

    struct ResponseGetCallHistory
    {
        ResponseGetCallHistory()= default;
        explicit ResponseGetCallHistory(const vector<callHistory> &callHistory): callHistories(callHistory) {}
        int code;
        vector<callHistory> callHistories;
    };

    struct BadResponse
    {
        int code;
        string body;
        BadResponse()
        {
            code = 400;
            body = R"({"error":"Invalid JSON. Expect {\"number\":number}."})";
        };
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(BadResponse, code, body)
    };

#endif //CONTACT_DDD_RESPONSE_DTO_HPP