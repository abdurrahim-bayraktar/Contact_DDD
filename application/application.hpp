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
        vector<callHistory>histories = callHistoryService_->getAllCallHistories(tx);
        for (int i = 0; callHistory& call : histories)
        {

            histories[i].callerName = contactService::getNameById(tx, call.callerId);

            histories[i].calleeName = contactService::getNameById(tx, call.calleeId);
            ++i;
        }
        return histories;
    };

    static void addContact(pqxx::work& tx, string& name, string& number, string& address)
    {
        cout << contactService::addContact(tx, name, number, address) << endl;
    };

    static void addCallHistory(pqxx::work& tx, string& callerNumber, string& CalleeNumber)
    {
        int callerId = contactService::getIdByNumber(tx, callerNumber);
        int calleeId = contactService::getIdByNumber(tx, CalleeNumber);

        string status = callHistoryService::addCallHistory(tx,callerId, calleeId);
        cout << status << endl;
    };
    void deleteContact(pqxx::work&, string& number){};
    void deleteCallHistory(pqxx::work&, int& callId){};
    void editContact(pqxx::work&, string& newName, string& number){};


};


#endif // APPLICATION_HPP
