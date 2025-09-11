#ifndef CONTACT_REPOSITORY_HPP
#define CONTACT_REPOSITORY_HPP
#include "contact.hpp"
#include "contact-factory.hpp"
#include "../../application/DTO/request-dto.hpp"
#include "../../application/DTO/response-dto.hpp"

namespace contactRepository
{
    inline ResponseGetContacts getAllRows(pqxx::nontransaction& tx)
    {
        pqxx::result rows = tx.exec("SELECT * FROM contacts");
        ResponseGetContacts responseDTO(contactFactory::createContactVector(rows));

        return responseDTO;
    };

    inline ResponseDTO addContact(pqxx::nontransaction& tx, const pqxx::params& params)
    {
        pqxx::result result = tx.exec("INSERT INTO contacts (Name, Number, addrress) VALUES($1, $2, $3) RETURNING ContactID", params);
        ResponseDTO response;
        response.code = 200;
        response.body = "{{'id': " + to_string(result[0][0].as<int>()) + "}}";

        return response;

    }

//deprecated
    inline string getNameById(pqxx::nontransaction& tx, const int id)
    {
        pqxx::params param {id};
        pqxx::result name = tx.exec("SELECT Name FROM contacts where ContactID = $1 LIMIT 1", param);

        return get<0>(name[0].as<string>());
    }

//deprecated
    inline int getIdByNumber(pqxx::nontransaction& tx, const string& number)
    {
        const pqxx::params param{number};
        const pqxx::result Id = tx.exec("SELECT ContactID FROM contacts WHERE Number = $1", param);


        if (Id[0][0].is_null())
        {
            cout << "ERROR: No such contact with ID";

            return -1;
        }

        return get<0>(Id[0].as<int>());
    }

    inline int editContact(pqxx::nontransaction& tx, const RequestEditContact& requestDTO)
    {
        const pqxx::params params = contactFactory::createEditContactParams(requestDTO.name, requestDTO.id);
        const pqxx::result rows = tx.exec("UPDATE contacts SET Name = $1 WHERE ContactID = $2", params);

        return rows.affected_rows();

    }

    inline int deleteContact(pqxx::nontransaction& tx, const RequestDeleteContact& requestDTO)
    {
        const pqxx::params param {requestDTO.contactId};
        pqxx::result rows = tx.exec("DELETE FROM contacts WHERE contactID = $1", param);

        return rows.affected_rows();
    }

    inline vector<int> getIdsFromNumbers(pqxx::nontransaction& tx, const vector<string>& numbers, const pqxx::params& params)
    {

        string queryString = "SELECT DISTINCT ON (Number) Number, ContactID FROM contacts WHERE Number = ANY (ARRAY[";

        for (unsigned int i = 0; i < params.size(); i++)
        {
            queryString += "$";
            queryString += std::to_string(i+1);

            if (i != params.size() - 1)
            {
                queryString += ", ";
            }

        }
        queryString += "]) ORDER BY Number, ContactID";

        pqxx::result rows = tx.exec(queryString, params);

        vector<int> idVector = contactFactory::createGetIdOrderedVector(rows, numbers);


        return idVector;
    }

    inline int getNamesByIds(pqxx::nontransaction& tx, unordered_map<int, string>& names, const pqxx::params& params)
    {
        string queryString = "SELECT Name, ContactID FROM contacts WHERE ContactID = ANY (ARRAY[";

        for (unsigned int i = 0; i < params.size(); i++)
        {
            queryString += "$";
            queryString += std::to_string(i+1);

            if (i != params.size() - 1)
            {
                queryString += ", ";
            }

        }
        queryString += "]::int[])ORDER BY Number, ContactID";

        pqxx::result rows = tx.exec(queryString, params);

        return contactFactory::createGetNamesByIdsMap(rows, names);
    }
};


#endif // CONTACT_REPOSITORY_HPP
