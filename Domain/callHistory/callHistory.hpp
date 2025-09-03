#ifndef CALLHISTORY_HPP
#define CALLHISTORY_HPP
using namespace std;
class callHistory
{
    public:
    int callId;
    int callerId;
    int calleeId;
    string date;

    callHistory();
    callHistory(int callId, int callerId, int calleeId, string date) : callId(callId),
                                callerId(callerId), calleeId(calleeId), date(date){};


};
#endif // CALLHISTORY_HPP
