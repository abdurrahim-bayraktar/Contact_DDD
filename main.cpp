#include <pqxx/pqxx>
#include <iostream>
#include <string>
#include "./application/application.hpp"
#include "./infrastructure/connectionPooling.hpp"
#include <regex>

bool phoneNumberIsValid(string phoneNumber)
{
    regex patern("^\\+[0-9]{2}-[0-9]{3}-[0-9]{3}-[0-9]{2}-[0-9]{2}$");
    return regex_match(phoneNumber, patern);
}


int main()
{

    //initialization
    string poolString{"dbname = contactsDB user = postgres password = postgres \
      hostaddr = 127.0.0.1 port = 5432"};

    connectionPool pool(poolString, 5);
    Application app;


    //menu:

    while (true)
    {
        system("clear");
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
                system("clear");

                app.printContacts(pool);

                pool.release(conn);
                break;

            }

        case 1: //print calls
            {
                auto conn = pool.acquire();
                pqxx::work tx(*conn);
                system("clear");
                app.printCalls(pool, contact);
                pool.release(conn);
                break;
            }

        case 2: //create a call
            {
                auto conn = pool.acquire();
                pqxx::work tx(*conn);
                system("clear");

                app.printContacts(pool);

                cout << "what is the calling number ID?\n";
                int callerID = 0;
                cin >> callerID;
                cin.ignore(1000,'\n');

                cout << "what is the ID of number being called\n";
                int calleeID;
                cin >> calleeID;
                cin.ignore(1000,'\n');

                app.addCall(pool, callerID, calleeID, contact);
                pool.release(conn);
                break;
            }

        case 3: //create a contact
            {
                auto conn = pool.acquire();
                pqxx::work tx(*conn);
                system("clear");

                cout << "creating new app...\n";
                cout << "write the name of the contact\n";
                string name = "";
                getline(cin, name);
                cout << "write the number of the contact\n Valid form is: +XX-XXX-XXX-XX-XX\n";
                string number = "";
                getline(cin, number);
                while (!phoneNumberIsValid(number))
                {
                    cout << "wrong format please try again! write break to exit\n";
                    getline(cin, number);
                    if (number.compare("break") == 0){break;}
                }
                app.addContact(pool, name, number);
                pool.release(conn);
                break;
            }

        case 4: //update a contact
            {
                auto conn = pool.acquire();
                pqxx::work tx(*conn);
                system("clear");

                cout << "what is the number of the contact you would like to change?\n";
                string contactToChange;
                getline(cin, contactToChange);

                cout <<"what would you like to change the name to?\n";
                string nameToChange;
                getline(cin, nameToChange);

                app.editContactByNumber(pool, 0, nameToChange, contactToChange);
                pool.release(conn);
                break;

            }

        case 5: // delete a call
            {
                auto conn = pool.acquire();
                pqxx::work tx(*conn);
                system("clear");
                app.printCalls(pool, contact);

                cout << "enter the ID of the call you would like to delete" << endl;

                int callID = 0;
                cin >> callID;
                cin.ignore(1000,'\n');

                app.deleteCall(pool, callID);
                pool.release(conn);
                break;
            }

        case 6: // delete a contact
            {
                auto conn = pool.acquire();
                pqxx::work tx(*conn);
                system("clear");
                app.printContacts(pool);

                cout << "Enter the number of the contact you would like to delete";
                string contactToDelete;
                cin >> contactToDelete;
                cin.ignore(1000,'\n');

                app.deleteContactByNumber(pool, contactToDelete);
                pool.release(conn);
                break;
            }
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