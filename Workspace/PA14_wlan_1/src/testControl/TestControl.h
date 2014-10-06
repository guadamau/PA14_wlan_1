/*
 * TestControl.h
 *
 *  Created on: 20.02.2009
 *      Author: gemp
 */

#ifndef TESTCONTROL_H_
#define TESTCONTROL_H_

#include <omnetpp.h>
#include <list>
#include "MACAddress.h"
#include "EtherFrame_m.h"
#include "vlanMessage_m.h"
#include "dataMessage_m.h"
#include "hsrMessage_m.h"
//#include "ListenErkennung.h"
#include "hsrDefines.h"


struct structmd
{
	MACAddress cpu;
    EthernetIIFrame *result_ethTag;
    vlanMessage     *result_vlanTag;
    hsrMessage     *result_hsrTag;
    dataMessage     *result_messageData;
    simtime_t time;
};
typedef struct structmd MessageData;

class TestControl : public cSimpleModule
{
private:
    //static TestControl *instance;



    std::list<MessageData> sendList;
    std::list<MessageData> recvList;
    std::list<MACAddress> cpuList;

    static bool compare_tx(MessageData first, MessageData second);
    static bool compare_rx(MessageData first, MessageData second);
    bool mdCompare(MessageData *mdSend, MessageData *mdRecv);
    void searchMessage(MessageData *md, MACAddress cpu);

protected:

    unsigned long numFramesReceivedOK;
    unsigned long numFramesReceivedFAIL;
    unsigned long numFramesReceivedDUP;

    virtual void initialize();
    virtual int numInitStages() const {return 2;};
    virtual void handleMessage(cMessage *msg);
    virtual void finish();

public:
    //static TestControl* getInstance();
    ///static void delInstance();

    TestControl();
    virtual ~TestControl();


    //void registerDuplicateEntry (ListItemData entry);

    bool registerCPU (MACAddress cpu);
    void registerSEND(MACAddress cpu, EthernetIIFrame *data, simtime_t time);
    void registerRECV(MACAddress cpu, EthernetIIFrame *data, simtime_t time);

    //void printStat();
};

#endif /* TESTCONTROL_H_ */
