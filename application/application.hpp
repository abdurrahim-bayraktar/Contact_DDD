#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "../Domain/callHistory/callHistory-service.hpp"
#include "../Domain/contact/contact-service.hpp"
#include "../Domain/contact/contact-factory.hpp"
using namespace std;

class application
{
    shared_ptr<contactService> contactService_;
    shared_ptr<callHistoryService> callHistoryService_;

public:
    application()
    {
        auto factory = make_shared<contactFactory>();
        auto repository = make_shared<contactRepository>();
        contactService_ = make_shared<contactService>(factory, repository);
    };

    shared_ptr<vector<contact>> getContacts(pqxx::work& tx)
    {
        return contactService_->getAllContact(tx);
    };

    vector<callHistory> getCallHistory(pqxx::work& tx)
    {
        return callHistoryService_->getAllCallHistories(tx);
    };

    void addContact(pqxx::work& tx, string& name, string& number, string& address)
    {
        cout << contactService_->addContact(tx, name, number, address) << endl;
    };

    void addCallHistory(pqxx::work&, string& callerNumber, string& CalleeNumber){};
    void deleteContact(pqxx::work&, string& number){};
    void deleteCallHistory(pqxx::work&, int& callId){};
    void editContact(pqxx::work&, string& newName, string& number){};


};


#endif // APPLICATION_HPP
