#ifndef CONTACT_REPOSITORY_HPP
#define CONTACT_REPOSITORY_HPP
#include "contact.hpp"
#include "contact-factory.hpp"

class contactRepository
{
    contactFactory factory_;
    public:
    contactRepository();
    shared_ptr<vector<contact>> getAllRows(pqxx::work& tx);
};

inline shared_ptr<vector<contact>> contactRepository::getAllRows(pqxx::work& tx)
{
    shared_ptr<pqxx::result> rows = make_shared<pqxx::result>(tx.exec("SELECT * FROM contact"));

    shared_ptr<vector<contact>> objects = make_shared<vector<contact>>();

    for (auto row : *rows)
    {
        objects->push_back(factory_.makeContactFromRow(row));
    }

    return objects;
}

#endif // CONTACT_REPOSITORY_HPP
