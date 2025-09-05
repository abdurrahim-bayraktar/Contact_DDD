#include <pqxx/pqxx>
#include <iostream>
#include <string>
#include "./application/application.hpp"
#include "./infrastructure/connectionPooling.hpp"
#include <regex>

bool phoneNumberIsValid(const string& phoneNumber)
{
    regex patern("^\\+[0-9]{2}-[0-9]{3}-[0-9]{3}-[0-9]{2}-[0-9]{2}$");
    return regex_match(phoneNumber, patern);
}

void tempUtility(pqxx::work& tx)
{
    tx.exec("ALTER TABLE contacts ADD COLUMN addrress varchar(50)");
    tx.commit();
};

void tempUtility2(pqxx::work& tx)
{
    tx.exec("UPDATE contacts SET addrress = 'a'");
    tx.commit();
}

void printContacts(const vector<contact>& contacts)
{
    cout << left << setw(3)<< "ID";
    cout<< "   ";
    cout << right <<setw(10) <<"Name" << " " << setw(17) <<"Number"<< " ";
    cout << setw(20) <<"address" << endl;

    for (auto& contact : contacts)
    {
        cout <<left << setw(4)<< contact.id;
        cout << "   ";
        cout << right << setw(10) << contact.name;
        cout << " " << setw(20) << contact.number;
        cout << " " << setw(15) << contact.address << endl;
    }
};

void printCallHistory(const vector<callHistory>& callHistories)
{
    cout << left << setw(3)<< "CALL ID";
    cout<< "   ";
    cout << right <<setw(10) <<"caller name" << " " << setw(17) <<"callee name"<< " ";
    cout << setw(20) <<"date" << endl;

    for (const auto& callHistory : callHistories)
    {
        cout <<left << setw(4)<< callHistory.callId;
        cout << "   ";
        cout << right << setw(10) << callHistory.callerName;
        cout << " " << setw(20) << callHistory.calleeName;
        cout << " " << setw(15) << callHistory.date << endl;
    }
};


int main()
{

    //initialization
    string poolString{"dbname = contactsDB user = postgres password = postgres \
      hostaddr = 127.0.0.1 port = 5432"};

    connectionPool pool(poolString, 5);

    //menu:
    while (true)
    {
        
        cout << "WELCOME TO CONTACTS APP."<< "\n PLEASE SELECT FROM BELOW MENU WHAT WOULD YOU LIKE TO DO" << endl;

        cout << "MENU:\n 0 = SEE CONTACTS\n 1 = SEE CALLS\n 2 = MAKE A CALL\n 3 = ADD A CONTACT\n \
4 = EDIT A CONTACT\n 5 = REMOVE A CALL\n 6 = DELETE A CONTACT\n";



        int menuSelector = 0;
        cin >> menuSelector;
        cin.ignore(1000,'\n');



        switch (menuSelector)
        {
        case 0: //print contacts
            {
                auto conn = pool.acquire();
                pqxx::work tx(*conn);

                auto contacts = application::getContacts(tx);

                printContacts(contacts);

                tx.commit();
                pool.release(conn);

                break;

            }

        case 1: //print calls
            {
                auto conn = pool.acquire();
                pqxx::work tx(*conn);
                
                auto callHistories = application::getCallHistory(tx);
                printCallHistory(callHistories);

                tx.commit();
                pool.release(conn);

                break;
            }

        case 2: //create a call
            {
                auto conn = pool.acquire();
                pqxx::work tx(*conn);

                auto contacts = application::getContacts(tx);
                printContacts(contacts);


                cout << "what is the calling number?\n";
                string callerID;
                getline(cin, callerID);

                cout << "what is the number being called\n";
                string calleeID;
                getline(cin, calleeID);

                application::addCallHistory(tx, callerID, calleeID);

                tx.commit();
                pool.release(conn);

                break;
            }

        case 3: //create a contact
            {
                auto conn = pool.acquire();
                pqxx::work tx(*conn);
                

                cout << "creating new contact...\n";
                cout << "write the name of the contact\n";
                string name;
                getline(cin, name);
                cout << "write the number of the contact\n Valid form is: +XX-XXX-XXX-XX-XX\n";
                string number;
                getline(cin, number);

                while (!phoneNumberIsValid(number))
                {
                    cout << "wrong format please try again! write break to exit\n";
                    getline(cin, number);
                    if (number == "break"){break;}
                }
                string address;
                cout << "write the address of the contact (char < 50)";
                getline(cin, address);
                application::addContact(tx, name, number, address);

                tx.commit();
                pool.release(conn);

                break;
            }

        case 4: //update a contact
            {
                auto conn = pool.acquire();
                pqxx::work tx(*conn);

                auto contacts = application::getContacts(tx);
                printContacts(contacts);

                cout << "what is the id of the contact you would like to change?\n";
                int contactId;
                cin >> contactId;
                cin.ignore(1000,'\n');

                cout <<"what would you like to change the name to?\n";
                string nameToChange;
                getline(cin, nameToChange);

                application::editContact(tx, nameToChange, contactId);

                tx.commit();
                pool.release(conn);

                break;

            }

        case 5: // delete a call
            {
                auto conn = pool.acquire();
                pqxx::work tx(*conn);
                
                auto callHistories = application::getCallHistory(tx);
                printCallHistory(callHistories);

                cout << "enter the ID of the call you would like to delete" << endl;

                int callID = 0;
                cin >> callID;
                cin.ignore(1000,'\n');

                application::deleteCallHistory(tx, callID);

                tx.commit();
                pool.release(conn);

                break;
            }

        case 6: // delete a contact
            {
                auto conn = pool.acquire();
                pqxx::work tx(*conn);

                auto contacts = application::getContacts(tx);
                printContacts(contacts);

                cout << "Enter the Number of the contact you would like to delete\n WARNING: RELATED CALLS WILL BE DELETED\n";
                string contactToDelete;
                cin >> contactToDelete;
                cin.ignore(1000,'\n');

                application::deleteContact(tx, contactToDelete);

                tx.commit();
                pool.release(conn);

                break;
            }
        default: ;
        }


        int againSelector = 0;
        cout<< "WOULD YOU LIKE TO SEE THE MENU AGAIN? 1 = YES, 0 = NO";

        cin >> againSelector;
        cin.ignore(1000,'\n');

        if (againSelector != 1)
        {
            break;
        }

    }
}