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
    contact* getAllContact(pqxx::work& tx);

};

inline contact* contactService::getAllContact(pqxx::work& tx)
{
    shared_ptr<vector<contact>> objects = make_shared<vector<contact>>();
    auto rows =  repository_->getAllRows(tx);
    for (auto row : *rows)
    {
        objects->push_back(factory_->makeContactFromRow(row));

    }

}

#endif // CONTACT_SERVICE_HPP
