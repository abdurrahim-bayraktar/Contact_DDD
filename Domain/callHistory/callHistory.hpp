
#ifndef CALLHISTORY_HPP
#define CALLHISTORY_HPP
#include <utility>
#include "value-objects/contact-info.hpp"
using namespace std;

class callHistory
{
    public:

    int callId;
    int otherContactId;
    bool isIncoming;
    contactInfo Info;
    string otherName;
    string date;

    callHistory();
    callHistory(const int& callId, const int& otherContactId, const bool isIncoming, string& date) : callId(callId),
                                otherContactId(otherContactId), isIncoming(isIncoming), date(std::move(date)){};
};
#endif // CALLHISTORY_HPP
