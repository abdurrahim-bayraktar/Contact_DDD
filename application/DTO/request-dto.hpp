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

struct RequestGetContacts
{

};

struct RequestAddCall
{

    RequestAddCall(const string& number, const bool& isIncoming) :
        number(number), id(0), isIncoming(isIncoming){}

    string number;
    int id;
    bool isIncoming;
};

struct RequestDeleteCall
{
    explicit RequestDeleteCall(const int& callId): callId(callId){}

    int callId;
};

struct RequestAddContact
{
    RequestAddContact(const string& name, const string& number, const string& address):
    name(name),number(number), address(address) {}

    string name;
    string number;
    string address;
};

struct RequestDeleteContact
{
    explicit RequestDeleteContact(const int& contactId): contactId(contactId) {}

    int contactId;
};

struct RequestEditContact
{
    RequestEditContact(const string& name, const int& id): name(name), id(id) {}

    string name;
    int id;
};
#endif //CONTACT_DDD_REQUEST_DTO_HPP