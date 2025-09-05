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



    // static vector<callHistory> getCallHistory(pqxx::work& tx)
    // {
    //     vector<callHistory>histories = callHistoryService::getAllCallHistories(tx);
    //
    //     for (int i = 0; callHistory& call : histories)
    //     {
    //         vector<int> ids = {call.callerId, call.calleeId};
    //         unordered_map<int, string> names;
    //         names[ids[0]];
    //         names[ids[1]];
    //
    //         contactService::getNamesByIds(tx, names);
    //
    //         histories[i].callerName = names[ids[0]];
    //         histories[i].calleeName = names[ids[1]];
    //         ++i;
    //     }
    //     return histories;
    // };

    inline vector<callHistory> getCallHistory(pqxx::work& tx)
    {
        vector<callHistory>histories = callHistoryService::getAllCallHistories(tx);

        for (int i = 0; callHistory& call : histories)
        {
            call.otherName = contactService::getNameById(tx, call.otherContactId);
            ++i;
        }
    return histories;

    }

    static void addContact(pqxx::work& tx, string& name, string& number, string& address)
    {
        cout << contactService::addContact(tx, name, number, address) << endl;
    };

    static void addCallHistory(pqxx::work& tx, const string& otherContactNumber, bool isIncoming)
    {



        int otherId = contactService::getIdByNumber(tx, otherContactNumber);

        string status = callHistoryService::addCallHistory(tx, otherId, isIncoming);
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
