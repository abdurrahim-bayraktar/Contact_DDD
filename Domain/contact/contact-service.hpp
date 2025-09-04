#ifndef CONTACT_SERVICE_HPP
#define CONTACT_SERVICE_HPP

#include "contact.hpp"
#include "contact-factory.hpp"
#include "contact-repository.hpp"
#include <vector>

using namespace std;


namespace contactService
{



    inline vector<contact> getAllContact(pqxx::work& tx)
    {

        return contactRepository::getAllRows(tx);
    };

    inline string addContact(pqxx::work& tx, string& name, string& number, string& address)
    {
        return contactRepository::addContact(tx, name, number, address);
    };

    inline string getNameById(pqxx::work& tx, const int id)
    {
        return contactRepository::getNameById(tx, id);
    }

    inline int getIdByNumber(pqxx::work& tx, const string& number)
    {
        return contactRepository::getIdByNumber(tx, number);
    }

    inline pair<int, int> getIdsByNumbers(pqxx::work& tx, const string& callerNumber, const string& calleeNumber)
    {
        return contactRepository::getIdsFromNumbers(tx, callerNumber, calleeNumber);
    }

    inline string editContact(pqxx::work& tx, const string& name, const int id)
    {
        return contactRepository::editContact(tx, name, id);
    }

    inline string deleteContact(pqxx::work& tx, const string& number)
    {
        return contactRepository::deleteContact(tx, number);
    }

};

#endif // CONTACT_SERVICE_HPP
