#ifndef CONTACT_SERVICE_HPP
#define CONTACT_SERVICE_HPP

#include "contact.hpp"
#include "contact-factory.hpp"
#include "contact-repository.hpp"
#include <vector>
#include <crow.h>
#include "../../application/DTO/request-dto.hpp"
#include "../../application/DTO/response-dto.hpp"

using namespace std;


namespace contactService
{



    inline ResponseGetContacts getAllContact(pqxx::work& tx)
    {

        return contactRepository::getAllRows(tx);
    };

    inline ResponseDTO addContact(pqxx::work& tx, RequestAddContact& dto)
    {
        pqxx::params params = contactFactory::createAddContactParam(dto);
        return contactRepository::addContact(tx, params);
    };

//deprecated
    inline string getNameById(pqxx::work& tx, const int id)
    {
        return contactRepository::getNameById(tx, id);
    }
//deprecated
    inline int getIdByNumber(pqxx::work& tx, const string& number)
    {
        return contactRepository::getIdByNumber(tx, number);
    }

    inline vector<int> getIdsByNumbers(pqxx::work& tx, const vector<string>& numbers)
    {
        const pqxx::params params = contactFactory::createGetIdsByNumbersParams(numbers);
        vector<int> idVector = contactRepository::getIdsFromNumbers(tx, numbers, params);

        if (idVector.empty())
        {
            cout << "ERROR: NO ID BY THOSE NUMBERS";
        }

        return idVector;

    }

    inline ResponseDTO editContact(pqxx::work& tx, const RequestEditContact& requestDTO)
    {
        ResponseDTO res;
        if (contactRepository::editContact(tx, requestDTO) == 0)
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

    inline crow::response deleteContact(pqxx::work& tx, const RequestDeleteContact& requestDTO)
    {
        crow::response res;
        if (contactRepository::deleteContact(tx, requestDTO) == 0 )
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

    //deprecated
    [[maybe_unused]] inline void getNamesByIds(pqxx::work& tx, unordered_map<int, string>& names)
    {
        const pqxx::params params = contactFactory::createGetNamesBydIdsParam(names);
        contactRepository::getNamesByIds(tx, names, params);
        if (names.empty())
        {
            cout << "ERROR: NO NAME BY IDS";
        }
    }
};

#endif // CONTACT_SERVICE_HPP
