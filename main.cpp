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
    application app;


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
                

                auto contacts = app.getContacts(tx);

                cout << left << setw(3)<< "ID";
                cout<< "   ";
                cout << right <<setw(10) <<"Name" << " " << setw(17) <<"Number"<< " ";
                cout << setw(20) <<"address" << endl;

                for (auto contact : *contacts)
                {
                    cout <<left << setw(4)<< contact.id;
                    cout << "   ";
                    cout << right << setw(10) << contact.name;
                    cout << " " << setw(27) << contact.number;
                    cout << " " << setw(25) << contact.address << endl;
                }

                pool.release(conn);
                break;

            }

        case 1: //print calls
            {
                auto conn = pool.acquire();
                pqxx::work tx(*conn);
                
                app.getCallHistory(tx);
                pool.release(conn);
                break;
            }

        case 2: //create a call
            {
                auto conn = pool.acquire();
                pqxx::work tx(*conn);
                

                app.getContacts(tx);

                cout << "what is the calling number ID?\n";
                string callerID;
                getline(cin, callerID);

                cout << "what is the ID of number being called\n";
                string calleeID;
                getline(cin, calleeID);

                app.addCallHistory(tx, callerID, calleeID);
                pool.release(conn);
                break;
            }

        case 3: //create a contact
            {
                auto conn = pool.acquire();
                pqxx::work tx(*conn);
                

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
                app.addContact(tx, name, number);
                pool.release(conn);
                break;
            }

        case 4: //update a contact
            {
                auto conn = pool.acquire();
                pqxx::work tx(*conn);
                

                cout << "what is the number of the contact you would like to change?\n";
                string contactToChange;
                getline(cin, contactToChange);

                cout <<"what would you like to change the name to?\n";
                string nameToChange;
                getline(cin, nameToChange);

                app.editContact(tx, nameToChange, contactToChange);
                pool.release(conn);
                break;

            }

        case 5: // delete a call
            {
                auto conn = pool.acquire();
                pqxx::work tx(*conn);
                
                app.getCallHistory(tx);

                cout << "enter the ID of the call you would like to delete" << endl;

                int callID = 0;
                cin >> callID;
                cin.ignore(1000,'\n');

                app.deleteCallHistory(tx, callID);
                pool.release(conn);
                break;
            }

        case 6: // delete a contact
            {
                auto conn = pool.acquire();
                pqxx::work tx(*conn);
                
                app.getContacts(tx);

                cout << "Enter the number of the contact you would like to delete";
                string contactToDelete;
                cin >> contactToDelete;
                cin.ignore(1000,'\n');

                app.deleteContact(tx, contactToDelete);
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