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
    factory_(std::move(factory)), repository_(std::move(repository)){};


    shared_ptr<vector<contact>> getAllContact(pqxx::work& tx);

    static string addContact(pqxx::work& tx, string& name, string& number, string& address)
    {
        return contactRepository::addContact(tx, name, number, address);
    };

    static string getNameById(pqxx::work& tx, const int id)
    {
        return contactRepository::getNameById(tx, id);
    }

};

inline shared_ptr<vector<contact>> contactService::getAllContact(pqxx::work& tx)
{

    return contactRepository::getAllRows(tx);
}

#endif // CONTACT_SERVICE_HPP
