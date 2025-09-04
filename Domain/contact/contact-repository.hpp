#ifndef CONTACT_REPOSITORY_HPP
#define CONTACT_REPOSITORY_HPP
#include "contact.hpp"
#include "contact-factory.hpp"

namespace contactRepository
{



    static vector<contact> getAllRows(pqxx::work& tx)
    {

        pqxx::result rows = tx.exec("SELECT * FROM contacts");
        vector<contact> objects = contactFactory::createContactVector(rows);

        return objects;
    };

    static string addContact(pqxx::work& tx, string& name, string& number, string& address)
    {
        pqxx::params params = contactFactory::createAddContactParam(name,number, address);
        tx.exec("INSERT INTO contacts (Name, Number, addrress) VALUES($1, $2, $3)", params );
        tx.commit();
        return "200 OK";
    }

    static string getNameById(pqxx::work& tx, const int id)
    {
        pqxx::params param = contactFactory::createGetNameByIdParam(id);
        pqxx::result name = tx.exec("SELECT Name FROM contacts where ContactID = $1 LIMIT 1", param);
        return get<0>(name[0].as<string>());
    }

    static int getIdByNumber(pqxx::work& tx, const string& number)
    {
        const pqxx::params param = contactFactory::createGetIdByNumberParams(number);
        const pqxx::result Id = tx.exec("SELECT ContactID FROM contacts WHERE Number = $1", param);
        tx.commit();

        return get<0>(Id[0].as<int>());

    }

    static string editContact(pqxx::work& tx, const string& name, const int id)
    {
        pqxx::params params = contactFactory::createEditContactParams(name, id);
        tx.exec("UPDATE contacts SET Name = $1 WHERE ContactID = $2", params);
        tx.commit();

        return "200 OK";
    }

    static string deleteContact(pqxx::work& tx, const string& number)
    {
        pqxx::params param = contactFactory::createDeleteContactParams(number);
        tx.exec("DELETE FROM contacts WHERE Number = $1", param);
        tx.commit();
        return "200 OK";
    }

};


#endif // CONTACT_REPOSITORY_HPP
