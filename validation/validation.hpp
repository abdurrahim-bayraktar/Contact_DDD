//
// Created by user on 9/12/25.
//

#ifndef CONTACT_DDD_VALIDATION_HPP
#define CONTACT_DDD_VALIDATION_HPP
#include <string>
#include <regex>
#include <nlohmann/json.hpp>
using namespace std;
using json = nlohmann::json;
namespace validation
{
    inline bool phoneNumberIsValid(const string& phoneNumber)
    {
        regex patern("^\\+[0-9]{2}-[0-9]{3}-[0-9]{3}-[0-9]{2}-[0-9]{2}$");
        return regex_match(phoneNumber, patern);
    }

    // inline bool isInRequest (const string& area, json& request)
    // {
    //
    // }
}


#endif //CONTACT_DDD_VALIDATION_HPP