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



    inline vector<contact> getAllContact(pqxx::nontransaction& tx)
    {

        return contactRepository::getAllRows(tx);
    };

    inline int addContact(pqxx::nontransaction& tx, const string& name, const string& number, const string& address)
    {
        pqxx::params params = contactFactory::createAddContactParam(name, number, address);
        return contactRepository::addContact(tx, params);
    };



    inline string getNameById(pqxx::nontransaction& tx, const int id)
    {
        return contactRepository::getNameById(tx, id);
    }

     inline int getIdByNumber(pqxx::nontransaction& tx, const string& number)
     {
         return contactRepository::getIdByNumber(tx, number);
     }

    inline vector<int> getIdsByNumbers(pqxx::nontransaction& tx, const vector<string>& numbers)
    {
        const pqxx::params params = contactFactory::createGetIdsByNumbersParams(numbers);
        vector<int> idVector = contactRepository::getIdsFromNumbers(tx, numbers, params);

        if (idVector.empty())
        {
            cout << "ERROR: NO ID BY THOSE NUMBERS";
        }

        return idVector;

    }

    inline int editContact(pqxx::nontransaction& tx, const string& name, const int& id)
    {

        return contactRepository::editContact(tx, name, id); // returns the number of changed rows
        // if (contactRepository::editContact(tx, name, id) == 0)
        // {
        //     res.code = 404;
        //     res.body =  "ERROR: id is not in db";
        //     return res;
        // }
        // else
        // {
        //     res.code = 200;
        //     res.body =  "200 OK";
        //     return res;
        // }
    }

    inline int deleteContact(pqxx::nontransaction& tx, const int& contactId)
    {
        return contactRepository::deleteContact(tx, contactId);
        // if (contactRepository::deleteContact(tx, requestDTO) == 0 )
        // {
        //     res.code = 404;
        //     res.body =  "ERROR: id is not in db";
        //     return res;
        // }
        // else
        // {
        //     res.code = 200;
        //     res.body =  "200 OK";
        //     return res;
        // }
    }

    //deprecated
    // [[maybe_unused]] inline void getNamesByIds(pqxx::nontransaction& tx, unordered_map<int, string>& names)
    // {
    //     const pqxx::params params = contactFactory::createGetNamesBydIdsParam(names);
    //     contactRepository::getNamesByIds(tx, names, params);
    //     if (names.empty())
    //     {
    //         cout << "ERROR: NO NAME BY IDS";
    //     }
    // }
};

#endif // CONTACT_SERVICE_HPP
