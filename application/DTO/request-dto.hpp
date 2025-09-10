//
// Created by user on 9/10/25.
//

#ifndef CONTACT_DDD_REQUEST_DTO_HPP
#define CONTACT_DDD_REQUEST_DTO_HPP
#include <string>
using namespace std;

class RequestDTO
{
public:

};
class CrudRequestDTO
{
    public:
    int id;
    string action;
    string name;
    string number;
    string address;
    bool isIncoming;

};


#endif //CONTACT_DDD_REQUEST_DTO_HPP