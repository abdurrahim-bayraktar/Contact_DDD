#ifndef CONTACT_REPOSITORY_HPP
#define CONTACT_REPOSITORY_HPP
#include "contact.hpp"
#include "contact-factory.hpp"

class contactRepository
{
    public:
    contactRepository();
    shared_ptr<pqxx::result> getAllRows(pqxx::work& tx);
};

inline shared_ptr<pqxx::result> contactRepository::getAllRows(pqxx::work& tx)
{
    shared_ptr<pqxx::result> rows = make_shared<pqxx::result>(tx.exec("SELECT * FROM contact"));
    return rows;
}

#endif // CONTACT_REPOSITORY_HPP
