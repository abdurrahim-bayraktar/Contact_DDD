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

    inline pqxx::params createGetIdsByNumbersParams(const string& callerNumber, const string& calleeNumber)
    {
        pqxx::params params{callerNumber, calleeNumber};
        return params;
    }

};


#endif // CONTACT_FACTORY_HPP
