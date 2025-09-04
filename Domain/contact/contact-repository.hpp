#ifndef CONTACT_REPOSITORY_HPP
#define CONTACT_REPOSITORY_HPP
#include "contact.hpp"
#include "contact-factory.hpp"

namespace contactRepository
{



    inline vector<contact> getAllRows(pqxx::work& tx)
    {
        pqxx::result rows = tx.exec("SELECT * FROM contacts");
        vector<contact> objects = contactFactory::createContactVector(rows);

        return objects;
    };

    inline string addContact(pqxx::work& tx, string& name, string& number, string& address)
    {
        pqxx::params params = contactFactory::createAddContactParam(name,number, address);
        tx.exec("INSERT INTO contacts (Name, Number, addrress) VALUES($1, $2, $3)", params );
        tx.commit();

        return "200 OK";
    }

    inline string getNameById(pqxx::work& tx, const int id)
    {
        pqxx::params param = contactFactory::createGetNameByIdParam(id);
        pqxx::result name = tx.exec("SELECT Name FROM contacts where ContactID = $1 LIMIT 1", param);

        return get<0>(name[0].as<string>());
    }

    inline int getIdByNumber(pqxx::work& tx, const string& number)
    {
        const pqxx::params param = contactFactory::createGetIdByNumberParams(number);
        const pqxx::result Id = tx.exec("SELECT ContactID FROM contacts WHERE Number = $1", param);
        tx.commit();

        if (Id[0][0].is_null())
        {
            cout << "ERROR: No such contact with ID";

            return -1;
        }

        return get<0>(Id[0].as<int>());

    }

    inline

    inline string editContact(pqxx::work& tx, const string& name, const int id)
    {
        pqxx::params params = contactFactory::createEditContactParams(name, id);
        pqxx::result rows = tx.exec("UPDATE contacts SET Name = $1 WHERE ContactID = $2", params);
        tx.commit();
        if (rows.affected_rows() != 0)
        {
            return "200 OK";
        }
        else {return "ERROR: ID not in db";}
    }

    inline string deleteContact(pqxx::work& tx, const string& number)
    {
        const pqxx::params param = contactFactory::createDeleteContactParams(number);
        pqxx::result rows =  tx.exec("DELETE FROM contacts WHERE Number = $1", param);
        tx.commit();
        if (rows.affected_rows() != 0)
        {
            return "200 OK";
        }
        else
        {
            return "ERROR: Number not in db";
        }
    }

    inline vector<int> getIdsFromNumbers(pqxx::work& tx, const vector<string>& numbers)
    {
        const pqxx::params params = contactFactory::createGetIdsByNumbersParams(numbers);

        const string queryString = contactFactory::createQueryStringGetIds(params.size());

        pqxx::result rows = tx.exec(queryString, params);

        vector<int> idVector = contactFactory::createGetIdsByNumbersVector(rows);
        tx.commit();

        return idVector;
    }



};


#endif // CONTACT_REPOSITORY_HPP
