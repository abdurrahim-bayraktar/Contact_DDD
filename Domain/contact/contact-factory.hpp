#ifndef CONTACT_FACTORY_HPP
#define CONTACT_FACTORY_HPP
#include "contact.hpp"

namespace contactFactory
{

    inline contact createContactFromRow(const int& id, const string& name, const string& number, const string& address)
    {
        contact contact(id, name, number, address);
        return contact;
    };

    inline vector<contact> createContactVector(const pqxx::result& rows)
    {
        auto objects = vector<contact>();

        for (auto row : rows)
        {
            auto[id, name, number, address] = row.as<int,string, string, string>();
            objects.push_back(contactFactory::createContactFromRow(id, name, number, address));
        }

        return objects;

    }

    inline pqxx::params createAddContactParam(string& name, string& number, string& address)
    {
        pqxx::params params{name,number, address};
        return params;
    }

    inline pqxx::params createGetNameByIdParam(const int id)
    {
        pqxx::params param{id};
        return param;

    };

    inline pqxx::params createGetIdByNumberParams(const string& number)
    {
        pqxx::params param{number};
        return param;
    }

    inline pqxx::params createEditContactParams(const string& name, const int id)
    {
        pqxx::params params{name,id};
        return params;
    }

    inline pqxx::params createDeleteContactParams(const string& number)
    {
        pqxx::params param{number};
        return param;
    }

    inline pqxx::params createGetIdsByNumbersParams(const vector<string>& numbers)
    {
        pqxx::params params{numbers[0]};

        for (int i=0; const string& number:numbers)
        {
            if (i == 0)
            {
                ++i;
                continue;
            }

            params.append(number);
            ++i;
        }

        return params;
    }

    inline vector<int> createGetIdsByNumbersVector(const pqxx::result& rows)
    {
        vector<int> ids;

        for (auto row : rows)
        {
            ids.push_back(row[0].as<int>());
        }

        return ids;
    }

    inline string createQueryStringGetIds(const int& parameterNumber)
    {

        string queryString = "SELECT DISTINCT ON (Number) ContactID FROM contacts WHERE Number = ANY (ARRAY[";

        for (unsigned int i = 0; i < parameterNumber; i++)
        {
            queryString += "$";
            queryString += std::to_string(i+1);

            if (i != parameterNumber - 1)
            {
                queryString += ", ";
            }

        }
        queryString += "]) ORDER BY Number, ContactID";

        return queryString;

    }

};


#endif // CONTACT_FACTORY_HPP
