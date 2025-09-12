#ifndef CONTACT_FACTORY_HPP
#define CONTACT_FACTORY_HPP
#include "contact.hpp"
#include "../../application/DTO/request-dto.hpp"
#include "../../application/DTO/response-dto.hpp"

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

    inline pqxx::params createAddContactParam(const string& name, const string& number,
        const string& address)
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

    inline pqxx::params createDeleteContactParams(const int& number)
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

    inline pqxx::params createGetNamesBydIdsParam(const unordered_map<int, string>& names)
    {
        pqxx::params params;

        for (int i = 0; pair p : names)
        {
            int key = p.first;
            params.append(key);
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



	inline vector<int> createGetIdOrderedVector(const pqxx::result& rows, const vector<string>& numbers)
    {
        vector<int> ids;

        for (auto row : rows)
        {
            string number = row["Number"].c_str();
            int id = row["ContactID"].as<int>();

            if (number == numbers[0])
                ids[0] = id;
            else if (number == numbers[1])
                ids[1] = id;
        }

        return ids;
    }

    inline int createGetNamesByIdsMap(const pqxx::result& rows, unordered_map<int, string>& names)
    {

        for (auto row : rows)
        {
            string name = row["Name"].c_str();
            int id = row["ContactID"].as<int>();

            names[id] = name;
        }

        return names.size();
    }

    inline string createQueryStringGetIds(const int& parameterNumber)
    {

        string queryString = "SELECT DISTINCT ON (Number) Number, ContactID FROM contacts WHERE Number = ANY (ARRAY[";

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

    inline string createQueryStringGetNames(const int& parameterNumber)
    {
        string queryString = "SELECT Name, ContactID FROM contacts WHERE ContactID = ANY (ARRAY[";

        for (unsigned int i = 0; i < parameterNumber; i++)
        {
            queryString += "$";
            queryString += std::to_string(i+1);

            if (i != parameterNumber - 1)
            {
                queryString += ", ";
            }

        }
        queryString += "]::int[])ORDER BY Number, ContactID";

        return queryString;
    }

};


#endif // CONTACT_FACTORY_HPP
