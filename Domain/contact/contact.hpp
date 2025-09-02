#ifndef CONTACT_HPP
#define CONTACT_HPP

#include <string>
using namespace std;

class contact
{
    int id;
    string name;
    string number;
    string adress;
public:
    contact();
    int getId();
    void setId(int id);
};
#endif // CONTACT_HPP
