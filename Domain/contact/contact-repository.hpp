#ifndef CONTACT_REPOSITORY_HPP
#define CONTACT_REPOSITORY_HPP
#include "contact.hpp"
#include "contact-factory.hpp"

class contactRepository
{
    contactFactory factory_;
    public:
    contactRepository()= default;


    static shared_ptr<vector<contact>> getAllRows(pqxx::work& tx);

    static string addContact(pqxx::work& tx, string& name, string& number, string& address)
    {
        pqxx::params params {name,number, address};
        tx.exec("INSERT INTO contacts (Name, Number, addrress) VALUES($1, $2, $3)", params );
        tx.commit();
        return "200 OK";
    }

    static string getNameById(pqxx::work& tx, const int id)
    {
        pqxx::params param = id;
        pqxx::result name = tx.exec("SELECT Name FROM contacts where ContactID = $1", param);
        return get<0>(name[0].as<string>());
    }

    static int getIdByNumber(pqxx::work& tx, const string& name)
    {
        pqxx::params param = name;
        pqxx::result Id = tx.exec("SELECT ContactID FROM contacts WHERE Name = $1", param);
        tx.commit();

        return get<0>(Id[0].as<int>());

    }
};

inline shared_ptr<vector<contact>> contactRepository::getAllRows(pqxx::work& tx)
{
    shared_ptr<pqxx::result> rows = make_shared<pqxx::result>(tx.exec("SELECT * FROM contacts"));

    shared_ptr<vector<contact>> objects = make_shared<vector<contact>>();

    for (auto row : *rows)
    {
        auto[id, name, number, address] = row.as<int,string, string, string>();
        objects->push_back(contactFactory::makeContactFromRow(id, name, number, address));
    }

    return objects;
}

#endif // CONTACT_REPOSITORY_HPP
