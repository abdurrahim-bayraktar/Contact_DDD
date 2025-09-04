#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "../Domain/callHistory/callHistory-service.hpp"
#include "../Domain/contact/contact-service.hpp"
#include "../Domain/contact/contact-factory.hpp"
using namespace std;

namespace application
{


    static vector<contact> getContacts(pqxx::work& tx)
    {
        return contactService::getAllContact(tx);
    };

    static vector<callHistory> getCallHistory(pqxx::work& tx)
    {
        vector<callHistory>histories = callHistoryService::getAllCallHistories(tx);
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

    static void deleteContact(pqxx::work& tx, string& number)
    {
        int contactId = contactService::getIdByNumber(tx, number);
        callHistoryRepository::deleteCallHistoriesWithContactId(tx, contactId);
        cout << contactRepository::deleteContact(tx, number) << endl;
    };

    void deleteCallHistory(pqxx::work& tx, int& callId)
    {
        cout << callHistoryRepository::deleteCallHistory(tx, callId) << endl;
    };

    static void editContact(pqxx::work& tx, string& newName, int& contactId)
    {
        cout << contactRepository::editContact(tx, newName, contactId)<< endl;
    };


};


#endif // APPLICATION_HPP
