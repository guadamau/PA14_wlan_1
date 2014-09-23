//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 2003 Ahmet Sekercioglu
// Copyright (C) 2003-2008 Andras Varga
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

#include <stdio.h>
#include <string.h>
#include <omnetpp.h>


// Include a generated file: the header file created from tictoc13.msg.
// It contains the definition of the TictocMsg10 class, derived from
// cMessage.
#include "tictoc13_m.h"


class Txc13 : public cSimpleModule
{
  private:
    cMessage *event;
  public:
    Txc13();
    virtual ~Txc13();
  protected:
    virtual TicTocMsg13 *generateMessage();
    virtual void forwardMessage(TicTocMsg13 *msg);
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
};

Define_Module(Txc13);

Txc13::Txc13()
{
   event = NULL;
}

Txc13::~Txc13()
{
   delete event;
}

void Txc13::initialize()
{

    event = new cMessage("event");
    simtime_t eventTime = par("eventTime");
    scheduleAt(simTime()+eventTime, event);
    EV << getIndex() << ": EventTimer activated, new one in " << eventTime << endl;

    // Module 0 sends the first message
    if (getIndex()==0)
    {
        // Boot the process scheduling the initial message as a self-message.
        TicTocMsg13 *msg = generateMessage();
        scheduleAt(0.0, msg);
    }
}

void Txc13::handleMessage(cMessage *msg)
{
    if (msg==event)
    {
	simtime_t eventTime = par("eventTime");
        scheduleAt(simTime()+eventTime, event);
	EV << getIndex() << ": EventTimer activated, new one in " << eventTime << endl;
	forwardMessage(generateMessage());
    }
    else
    {
	    TicTocMsg13 *ttmsg = check_and_cast<TicTocMsg13 *>(msg);

	    if (ttmsg->getDestination()==getIndex())
	    {
		// Message arrived.
		EV << getIndex() << ": Message " << ttmsg << " arrived after " << ttmsg->getHopCount() << " hops.\n";
		bubble("ARRIVED, starting new one!");
		delete ttmsg;

		// Generate another one.
		EV << getIndex() << ": Generating another message: ";
		TicTocMsg13 *newmsg = generateMessage();
		EV << newmsg << endl;
		forwardMessage(newmsg);
	    }
	    else
	    {
		// We need to forward the message.
		forwardMessage(ttmsg);
	    }
    }
}

TicTocMsg13 *Txc13::generateMessage()
{
    // Produce source and destination addresses.
    int src = getIndex();   // our module index
    int n = size();      // module vector size
    int dest = intuniform(0,n-2);
    if (dest>=src) dest++;

    char msgname[20];
    sprintf(msgname, "tic-%d-to-%d", src, dest);

    // Create message object and set source and destination field.
    TicTocMsg13 *msg = new TicTocMsg13(msgname);
    msg->setSource(src);
    msg->setDestination(dest);
    return msg;
}

void Txc13::forwardMessage(TicTocMsg13 *msg)
{
    cGate *arrivalGate = msg->getArrivalGate();
    
    // arrivalGate will be NULL if msg is new or self-message
    // if not NULL then increase hop count
    if(arrivalGate!=NULL) {
	    // Increment hop count.
	    EV << getIndex() << ": Increasing hop count to " << msg->getHopCount()+1 << endl;
	    msg->setHopCount(msg->getHopCount()+1);
    }

    // Same routing as before: random gate.
    int n = gateSize("gate");
    int k = intuniform(0,n-1);

    // If more than one port, don't send to same node whom it just received the msg
    if(n > 1)
    {
            if(arrivalGate!=NULL)
            {
                    int indexArrival = arrivalGate->getIndex();
                    while(indexArrival==k){
                            EV <<  getIndex() << ": Not Forwarding to same node again, choosing other gate..." << "\n";
                            k = intuniform(0,n-1);
                    }
            }
    }

    EV << getIndex() << ": Forwarding message " << msg << " on gate[" << k << "]\n";
    send(msg, "gate$o", k);
}
