#ifndef CONTACT_FACTORY_HPP
#define CONTACT_FACTORY_HPP
#include "contact.hpp"

class contactFactory
{
    public:
    contactFactory()
    {
    };
    static contact createContactFromRow(int id, const string& name, const string& number, const string& address)
    {
        contact contact(id, name, number, address);
        return contact;
    };


};


#endif // CONTACT_FACTORY_HPP
