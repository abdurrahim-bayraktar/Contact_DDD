#ifndef CONTACT_HPP
#define CONTACT_HPP

#include <string>
using namespace std;

class Contact
{
    int id;
    string name;
    string number;
    string adress;
public:
    Contact();
    int getId();
    void setId(int id;
};
#endif // CONTACT_HPP
