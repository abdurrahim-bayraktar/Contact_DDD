
#ifndef CALLHISTORY_HPP
#define CALLHISTORY_HPP
#include <utility>
#include "value-objects/contact-info.hpp"
using namespace std;

class callHistory
{
    public:

    int callId;
    int callerId;
    int calleeId;
    ContactInfo contactInfo;
    string callerName;
    string calleeName;
    string date;

    callHistory();
    callHistory(const int& callId, const int& callerId, const int& calleeId, string& date) : callId(callId),
                                callerId(callerId), calleeId(calleeId), date(std::move(date)){};
};
#endif // CALLHISTORY_HPP
