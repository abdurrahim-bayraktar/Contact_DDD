#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "Domain/callHistory/callHistory-service.hpp"
#include "Domain/contact/contact-service.hpp"
using namespace std;

class Application
{
    public:
    Application();
    void printContacts(pqxx::work);
    void

};
#endif // APPLICATION_HPP
