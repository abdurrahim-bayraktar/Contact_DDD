#ifndef CONTACT_FACTORY_HPP
#define CONTACT_FACTORY_HPP
#include "contact.hpp"

class contactFactory
{
    public:
    contactFactory();
    contact makeContactFromRow(pqxx::row);
};

inline contact contactFactory::makeContactFromRow(pqxx::row row)
{
    contact contact(row[1].as<string>(), row[2].as<string>(), row[3].as<string>());
    return contact;
}
#endif // CONTACT_FACTORY_HPP
