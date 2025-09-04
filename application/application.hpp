#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "../Domain/callHistory/callHistory-service.hpp"
#include "../Domain/contact/contact-service.hpp"
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
            //TODO:handled with double leftjoin on foreign keys
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

    static void addCallHistory(pqxx::work& tx, const string& callerNumber, const string& CalleeNumber)
    {

        vector<string> numbers;
        numbers.push_back(callerNumber);
        numbers.push_back(CalleeNumber);

        vector<int> ids = contactService::getIdsByNumbers(tx, numbers);

        if (ids.empty())
        {
            return;
        }

        int callerId = ids[0]; //is it certain these two won't be switched?
        int calleeId = ids[1];

        string status = callHistoryService::addCallHistory(tx,callerId, calleeId);
        cout << status << endl;

    };

    static void deleteContact(pqxx::work& tx, const string& number)
    {
        int contactId = contactService::getIdByNumber(tx, number);
        if (contactId != -1)
        {
            callHistoryRepository::deleteCallHistoriesWithContactId(tx, contactId);
            cout << contactRepository::deleteContact(tx, number) << endl;
        }

    };

    inline void deleteCallHistory(pqxx::work& tx, const int& callId)
    {
        cout << callHistoryService::deleteCallHistory(tx, callId) << endl;
    };

    static void editContact(pqxx::work& tx, const string& newName, const int& contactId)
    {
        cout << contactService::editContact(tx, newName, contactId)<< endl;
    };


};


#endif // APPLICATION_HPP
