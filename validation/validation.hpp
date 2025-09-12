//
// Created by user on 9/12/25.
//

#ifndef CONTACT_DDD_VALIDATION_HPP
#define CONTACT_DDD_VALIDATION_HPP
#include <string>
#include <regex>
using namespace std;

namespace validation
{
    bool phoneNumberIsValid(const string& phoneNumber)
    {
        regex patern("^\\+[0-9]{2}-[0-9]{3}-[0-9]{3}-[0-9]{2}-[0-9]{2}$");
        return regex_match(phoneNumber, patern);
    }
}


#endif //CONTACT_DDD_VALIDATION_HPP