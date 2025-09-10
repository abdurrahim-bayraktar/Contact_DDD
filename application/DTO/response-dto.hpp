//
// Created by user on 9/10/25.
//

#ifndef CONTACT_DDD_RESPONSE_DTO_HPP
#define CONTACT_DDD_RESPONSE_DTO_HPP
#include <string>
using namespace std;


class ResponseDTO
{
public:
    int code;
    string body;
};

class ContactResponseDTO
{

public:
    int id;
    string name;
    string number;
    string address;

};
#endif //CONTACT_DDD_RESPONSE_DTO_HPP