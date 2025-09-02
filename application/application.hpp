#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "../Domain/callHistory/callHistory-service.hpp"
#include "../Domain/contact/contact-service.hpp"
using namespace std;

class application
{
    shared_ptr<contactService> contactService_;
    shared_ptr<callHistoryService> callHistoryService_;

public:
    application();
    shared_ptr<vector<contact>> getContacts(pqxx::work&);
    callHistory* getCallHistory(pqxx::work&);
    void addContact(pqxx::work&, string& name, string& number);
    void addCallHistory(pqxx::work&, string& callerNumber, string& CalleeNumber);
    void deleteContact(pqxx::work&, string& number);
    void deleteCallHistory(pqxx::work&, int& callId);
    void editContact(pqxx::work&, string& newName, string& number);


};

inline application::application()
{
    auto factory = make_shared<contactFactory>();
    auto repository = make_shared<contactRepository>();
    contactService_ = make_shared<contactService>(factory, repository);
}

inline shared_ptr<vector<contact>> application::getContacts(pqxx::work& tx)
{

    return contactService_->getAllContact(tx);
}

inline callHistory* application::getCallHistory(pqxx::work&)
{
}

inline void application::addContact(pqxx::work&, string& name, string& number)
{
}

inline void application::addCallHistory(pqxx::work&, string& callerNumber, string& CalleeNumber)
{
}

inline void application::deleteContact(pqxx::work&, string& number)
{
}

inline void application::deleteCallHistory(pqxx::work&, int& callId)
{
}

inline void application::editContact(pqxx::work&, string& newName, string& number)
{
}

#endif // APPLICATION_HPP
