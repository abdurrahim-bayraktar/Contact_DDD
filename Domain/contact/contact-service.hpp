#ifndef CONTACT_SERVICE_HPP
#define CONTACT_SERVICE_HPP

#include "contact.hpp"
#include "contact-factory.hpp"
#include "contact-repository.hpp"

using namespace std;


class contactService
{
    shared_ptr<contactFactory> factory_;
    shared_ptr<contactRepository> repository_;
    public:
    contactService(shared_ptr<contactFactory> factory, shared_ptr<contactRepository> repository) :
    factory_(move(factory)), repository_(move(repository)){};
    contact* getAllContact(pqxx::work);

};

inline contact* contactService::getAllContact(pqxx::work)
{

}

#endif // CONTACT_SERVICE_HPP
