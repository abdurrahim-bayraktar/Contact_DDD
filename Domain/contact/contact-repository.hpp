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
    pqxx::result
}

#endif // CONTACT_REPOSITORY_HPP
