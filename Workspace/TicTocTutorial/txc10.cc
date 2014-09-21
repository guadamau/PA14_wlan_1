#include <stdio.h>
#include <string.h>
#include <omnetpp.h>


class Txc10 : public cSimpleModule
{
  protected:
    virtual void forwardMessage(cMessage *msg);
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
};

Define_Module(Txc10);

void Txc10::initialize()
{
    if (getIndex()==0)
    {
        // Boot the process scheduling the initial message as a self-message.
        char msgname[20];
        sprintf(msgname, "tic-%d", getIndex());
        cMessage *msg = new cMessage(msgname);
	scheduleAt(0.0, msg);
    }
}

void Txc10::handleMessage(cMessage *msg)
{
    if (getIndex()==3)
    {
        // Message arrived.
        EV << "Message " << msg << " arrived.\n";
        delete msg;
    }
    else
    {
        // We need to forward the message.
        forwardMessage(msg);
    }
}

void Txc10::forwardMessage(cMessage *msg)
{
    // In this example, we just pick a random gate to send it on.
    // We draw a random number between 0 and the size of gate `out[]'.
    int n = gateSize("out");
    int k = intuniform(0,n-1);

    // If more than one port, don't send to same node whom it just received the msg
    if(n > 1)
    {
    	    cGate *arrivalGate = msg->getArrivalGate();
	    if(arrivalGate!=NULL)
	    {
		    int indexArrival = arrivalGate->getIndex();
		    while(indexArrival==k){
			    EV << "Not Forwarding to same node again, choosing other gate..." << "\n";
			    k = intuniform(0,n-1);
		    }
	    }
    }

    EV << "Forwarding message " << msg << " on port out[" << k << "]\n";
    send(msg, "out", k);
}

