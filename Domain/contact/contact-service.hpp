#ifndef CONTACT_SERVICE_HPP
#define CONTACT_SERVICE_HPP

#include "contact.hpp"
#include "contact-factory.hpp"
#include "contact-repository.hpp"
#include <vector>

using namespace std;


class contactService
{
    shared_ptr<contactFactory> factory_;
    shared_ptr<contactRepository> repository_;
    public:
    contactService(shared_ptr<contactFactory> factory, shared_ptr<contactRepository> repository) :
    factory_(move(factory)), repository_(move(repository)){};


    shared_ptr<vector<contact>> getAllContact(pqxx::work& tx);
    string addContact(pqxx::work& tx, string& name, string& number, string& address)
    {
        return repository_->addContact(tx, name, number, address);
    };

};

inline shared_ptr<vector<contact>> contactService::getAllContact(pqxx::work& tx)
{

    return repository_->getAllRows(tx);
}

#endif // CONTACT_SERVICE_HPP
