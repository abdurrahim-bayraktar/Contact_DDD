//
// Created by user on 9/10/25.
//

#ifndef CONTACT_DDD_REQUEST_DTO_HPP
#define CONTACT_DDD_REQUEST_DTO_HPP
#include <string>
#include <nlohmann/json.hpp>
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
namespace addCall
{
    struct RequestAddCall
    {

        RequestAddCall(const string& number, const bool& isIncoming) :
            number(number), isIncoming(isIncoming){}

        string number;
        int id{0};
        bool isIncoming;
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(RequestAddCall, number, id, isIncoming)
    };
}

namespace deleteCall
{
    struct RequestDeleteCall
    {
        explicit RequestDeleteCall(const int& callId): callId(callId){}

        int callId;
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(RequestDeleteCall, callId)
    };
}

namespace AddContact
{
    struct RequestAddContact
    {
        RequestAddContact(const string& name, const string& number, const string& address):
        name(name),number(number), address(address) {}

        string name;
        string number;
        string address;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(RequestAddContact, name, number, address)
    };
}

namespace delteContact
{
    struct RequestDeleteContact
    {
        explicit RequestDeleteContact(const int& contactId): contactId(contactId) {}

        int contactId;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(RequestDeleteContact, contactId)
    };
}

namespace EditContact
{
    struct RequestEditContact
    {
        RequestEditContact(const string& name, const int& id): name(name), id(id) {}

        string name;
        int id;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(RequestEditContact, name, id)
    };
}
#endif //CONTACT_DDD_REQUEST_DTO_HPP