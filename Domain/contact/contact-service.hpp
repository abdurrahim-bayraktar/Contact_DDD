#ifndef CONTACT_SERVICE_HPP
#define CONTACT_SERVICE_HPP

#include "contact.hpp"
#include "contact-factory.hpp"
#include "contact-repository.hpp"
#include <vector>
#include <crow.h>

using namespace std;


namespace contactService
{



    inline vector<contact> getAllContact(pqxx::work& tx)
    {

        return contactRepository::getAllRows(tx);
    };

    inline ResponseDTO addContact(pqxx::work& tx, CrudRequestDTO dto)
    {
        return contactRepository::addContact(tx, dto.name, dto.number, dto.address);
    };

    inline string getNameById(pqxx::work& tx, const int id)
    {
        return contactRepository::getNameById(tx, id);
    }

    inline int getIdByNumber(pqxx::work& tx, const string& number)
    {
        return contactRepository::getIdByNumber(tx, number);
    }

    inline vector<int> getIdsByNumbers(pqxx::work& tx, const vector<string>& numbers)
    {
        vector<int> idVector = contactRepository::getIdsFromNumbers(tx, numbers);

        if (idVector.empty())
        {
            cout << "ERROR: NO ID BY THOSE NUMBERS";
        }

        return idVector;

    }

    inline crow::response editContact(pqxx::work& tx, const string& name, const int id)
    {
        crow::response res;
        if (contactRepository::editContact(tx, name, id) == 0)
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

    inline crow::response deleteContact(pqxx::work& tx, const int& number)
    {
        crow::response res;
        if (contactRepository::deleteContact(tx, number) == 0 )
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
        contactRepository::getNamesByIds(tx, names);
        if (names.empty())
        {
            cout << "ERROR: NO NAME BY IDS";
        }
    }
};

#endif // CONTACT_SERVICE_HPP
