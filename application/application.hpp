#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "Domain/callHistory/callHistory-service.hpp"
#include "Domain/contact/contact-service.hpp"
using namespace std;

class application : public
{
    shared_ptr<contactService> contactService_;
    shared_ptr<callHistoryService> callHistoryService_;

public:
    application();
    contact* printContacts(pqxx::work);
    callHistory* printCallHistory(pqxx::work);
    void addContact(pqxx::work, string name, string number);
    void addCallHistory(pqxx::work, string callerNumber, string CalleeNumber);
    void deleteContact(pqxx::work, string number);
    void deleteCallHistory(pqxx::work, int callId);
    void editContact(pqxx::work, string newName, string number);


};

inline contact* application::printContacts(pqxx::work tx)
{

    return contactService_->getAllContact(tx);
}

#endif // APPLICATION_HPP
