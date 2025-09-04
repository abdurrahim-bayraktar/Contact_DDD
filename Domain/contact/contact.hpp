#ifndef CONTACT_HPP
#define CONTACT_HPP

#include <string>
#include <utility>
using namespace std;

class contact
{
public:
    int id = 0;
    string name;
    string number;
    string address;

    contact();
    contact(int id, string  name, string  number, string  address);

};

inline contact::contact(const int id, string  name, string  number, string  address):
                            id(id), name(std::move(name)), number(std::move(number)), address(std::move(address))
{}


#endif // CONTACT_HPP
