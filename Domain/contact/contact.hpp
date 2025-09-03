#ifndef CONTACT_HPP
#define CONTACT_HPP

#include <string>
using namespace std;

class contact
{
public:
    int id = 0;
    string name;
    string number;
    string address;

    contact();
    contact(const int id, const string& name, const string& number, const string& address);

};

inline contact::contact(const int id, const string& name, const string& number, const string& address):
                            id(id), name(name), number(number), address(address)
{}


#endif // CONTACT_HPP
