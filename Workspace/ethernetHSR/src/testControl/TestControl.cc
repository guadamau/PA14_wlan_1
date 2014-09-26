/*
 * TestControl.cc
 *
 *  Created on: 20.02.2009
 *      Author: gemp
 */

#include "TestControl.h"
#include <utility> // make_pair
#include "hsrDefines.h"
#include "MessagePacker.h"

// #define CHECK_FRAME_DELIVERY

Define_Module(TestControl);

TestControl::TestControl()
{
    // TODO Auto-generated constructor stub
}

TestControl::~TestControl()
{
    // TODO Auto-generated destructor stub
}


void TestControl::registerSEND(MACAddress cpu, EthernetIIFrame *data, simtime_t time)
{
#ifdef CHECK_FRAME_DELIVERY
    MessageData md;
    md.cpu = cpu;
    md.time = time;
    md.result_ethTag = data;
    md.result_hsrTag = NULL;
    md.result_messageData = NULL;
    md.result_vlanTag = NULL;

    MessagePacker::decapsulateMessage(&(md.result_ethTag), &(md.result_vlanTag), &(md.result_hsrTag), &(md.result_messageData));

    //EV << "registerSEND CPU " << cpu << ": source: "  << md.source << " destination: "  << md.destination << " data "  << md.data << " sendtime: "  << md.time << " senderCPU: "  << md.cpu   << " ";
    sendList.push_back(md);
#else
    EthernetIIFrame *ethTag = check_and_cast<EthernetIIFrame *> (data);
    vlanMessage *vlanTag = NULL;
    hsrMessage *hsrTag = NULL;
    dataMessage *messageData = NULL;

    MessagePacker::decapsulateMessage(&ethTag, &vlanTag, &hsrTag, &messageData);
    MessagePacker::deleteMessage(&ethTag, &vlanTag, &hsrTag, &messageData);
#endif
}

void TestControl::registerRECV(MACAddress cpu, EthernetIIFrame *data, simtime_t time)
{
#ifdef CHECK_FRAME_DELIVERY
    bool hitFlag;
    std::list<MessageData>::iterator sIt; //recv
    MessageData md;
    md.cpu = cpu;
    md.time = time;
    md.result_ethTag = data;
    md.result_hsrTag = NULL;
    md.result_messageData = NULL;
    md.result_vlanTag = NULL;

    MessagePacker::decapsulateMessage(&(md.result_ethTag), &(md.result_vlanTag),&(md.result_hsrTag) , &(md.result_messageData));


    //EV << "registerRECV CPU " << cpu << ": source: "  << md.source << " destination: "  << md.destination << " data "  << md.data << " sendtime: "  << md.time << " senderCPU: "  << md.cpu   << " ";
 //   EV << " \r\n";;



//    //suche passende empfangsnachrichten
//    hitFlag = false;
//    for( sIt = sendList.begin(); sIt != sendList.end(); ++sIt )
//    {
//        //&(* ist nötig, um an den Inhalt des Iterators zu kommen
//        if(mdCompare(&(*sIt),&md) == true)
//        {
//            //treffer
//            hitFlag = true;
//            break;
//        }
//    }
//    if(hitFlag == false)
//    {
//        EV << "PAKET OHNE ABSENDER ANGEKOMMEN BEI CPU " << cpu << ": source: "  << md.result_ethTag->getSrc() << " destination: "  << md.result_ethTag->getDest() << " sendtime: "  << md.time << " senderCPU: "  << md.cpu   << " \n";
//        error("PAKET OHNE ABSENDER ANGEKOMMEN BEI CPU");
//    }
//    else
//    {
//    	//Alles ok, sender gefunden
//    	//EV << "Alles ok, sender gefunden CPU " << cpu << ": source: "  << md.source << " destination: "  << md.destination << " data "  << md.data << " sendtime: "  << md.time << " senderCPU: "  << md.cpu   << " ";
//    	//recordDelay(&(*sIt), &md, cpu);
//    }
    recvList.push_back(md);
#else
    EthernetIIFrame *ethTag = check_and_cast<EthernetIIFrame *> (data);
    vlanMessage *vlanTag = NULL;
    hsrMessage *hsrTag = NULL;
    dataMessage *messageData = NULL;

    MessagePacker::decapsulateMessage(&ethTag, &vlanTag, &hsrTag, &messageData);
    MessagePacker::deleteMessage(&ethTag, &vlanTag, &hsrTag, &messageData);

#endif
}

bool TestControl::registerCPU (MACAddress cpu)
{
#ifdef CHECK_FRAME_DELIVERY
    std::list<MACAddress>::iterator it;
    for( it = cpuList.begin(); it != cpuList.end(); ++it )
    {
        if(*it == cpu)
        {
            return false;
        }
    }

    cpuList.push_back(cpu);
#endif
    return true;
}

bool TestControl::mdCompare(MessageData *mdSend, MessageData *mdRecv)
{
/*
    EV << " COMPARE: "<< " \n";
    EV << " source: " << mdSend->result_ethTag->getSrc() << " destination: "  << mdSend->result_ethTag->getDest() << " data "  << mdSend->result_messageData->getCounter() << " sendtime: "  << mdSend->time << " vlanTag " << (void*)(mdRecv->result_vlanTag) << " \n";
    EV << " source: " << mdRecv->result_ethTag->getSrc() << " destination: "  << mdRecv->result_ethTag->getDest() << " data "  << mdRecv->result_messageData->getCounter() << " sendtime: "  << mdRecv->time << " vlanTag " << (void*)(mdRecv->result_vlanTag) << " \n";
    EV << " \n";
*/

    if((mdSend->time <= mdRecv->time) && (mdSend->result_ethTag->getSrc() == mdRecv->result_ethTag->getSrc()) && (mdSend->result_ethTag->getDest() == mdRecv->result_ethTag->getDest()) && (mdSend->result_messageData->getCounter() == mdRecv->result_messageData->getCounter()) && (((mdSend->result_vlanTag == NULL) && (mdRecv->result_vlanTag == NULL)) || ((mdSend->result_vlanTag != NULL) && (mdRecv->result_vlanTag != NULL))))
    {
        if(mdSend->result_vlanTag != NULL)
        {
            if((mdSend->result_vlanTag->getUser_priority() == mdRecv->result_vlanTag->getUser_priority()) && (mdSend->result_vlanTag->getCanonical_format_indicator() == mdRecv->result_vlanTag->getCanonical_format_indicator()) && (mdSend->result_vlanTag->getVlan_identifier() == mdRecv->result_vlanTag->getVlan_identifier()))
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return true;
        }
    }
    return false;
}

void TestControl::searchMessage(MessageData *sourceMd, MACAddress cpu)
{

    bool hitFlag;

    std::list<MessageData>::iterator rIt; //recv

    //suche passende empfangsnachrichten
    hitFlag = false;
    for( rIt = recvList.begin(); rIt != recvList.end(); ++rIt )
    {
    	MessageData *recvMd = &(*rIt);
        //&(* ist nötig, um an den Inhalt des Iterators zu kommen
        if((mdCompare(sourceMd,recvMd) == true) && (cpu == recvMd->cpu))
        {
            //treffer
        	if(recvMd->result_ethTag != NULL)
        		delete recvMd->result_ethTag;
            if(recvMd->result_vlanTag != NULL)
                delete recvMd->result_vlanTag;
            if(recvMd->result_hsrTag != NULL)
                delete recvMd->result_hsrTag;
            if(recvMd->result_messageData != NULL)
                delete recvMd->result_messageData;

            numFramesReceivedOK++;
        	//recordDelay(&(*sIt),&(*rIt),cpu);
            recvList.erase(rIt);
            hitFlag = true;
            break;
        }
    }
    if(hitFlag == false)
    {
    	numFramesReceivedFAIL++;
        //EV << "PAKET NICHT ANGEKOMMEN BEI CPU " << cpu << ": source: "  << sourceMd->result_ethTag->getSrc() << " destination: "  << sourceMd->result_ethTag->getDest() << " sendtime: "  << sourceMd->time << " senderCPU: "  << sourceMd->cpu  << " \n";
    }
}

void TestControl::initialize()
{
	numFramesReceivedOK   = 0;
	numFramesReceivedFAIL = 0;
	numFramesReceivedDUP  = 0;

    WATCH(numFramesReceivedOK);
    WATCH(numFramesReceivedFAIL);
    WATCH(numFramesReceivedDUP);

}


void TestControl::handleMessage(cMessage *msg)
{
	delete msg;
}

// comparison, not case sensitive.
bool TestControl::compare_tx(MessageData first, MessageData second)
{
	if(first.cpu < second.cpu)
	{
		if(first.result_ethTag->getDest() < second.result_ethTag->getDest())
		{
			return true;
		}
	}
	return false;
}

bool TestControl::compare_rx(MessageData first, MessageData second)
{
	if(first.result_ethTag->getSrc() < second.result_ethTag->getSrc())
	{
		if(first.result_ethTag->getDest() < second.result_ethTag->getDest())
		{
			return true;
		}
	}
	return false;
}


void TestControl::finish()
{
EV << "Ende Testcontrol sendList: " << sendList.size() << " recvList: " << recvList.size() << " cpuList: " << cpuList.size() << "\n";

sendList.sort(compare_tx);
cpuList.sort();
recvList.sort(compare_rx);

EV << "End sort \n";

	    //struct structmd md;
	    std::list<MACAddress>::iterator cIt; //cpu
	    std::list<MessageData>::iterator rIt; //recv
	    std::list<MessageData>::iterator sIt; //send
#ifdef CHECK_FRAME_DELIVERY
	    //gehe durch alle gesendeten Nachrichten
	    for( sIt = sendList.begin(); sIt != sendList.end(); ++sIt )
	    {
            for( cIt = cpuList.begin(); cIt != cpuList.end(); ++cIt )
            {
				if((sIt->result_ethTag->getDest().isBroadcast()) || (sIt->result_ethTag->getDest().isMulticast()))
				{
	            //Broadcast
	                //für alle registrierten CPUs
	                if(*cIt != sIt->result_ethTag->getSrc() )
	                {
	                    //ohne den Absender
	                    searchMessage(&(*sIt), *cIt);
	                }
	            }
				else
				{
	                //nur checken, wenn es den Zielknoten überhaupt gibt
	                if(*cIt == sIt->result_ethTag->getDest() )
	                {
	                    searchMessage(&(*sIt), *cIt);
	                    break;
	                }
				}
            }
	    }
#endif


	for( sIt = sendList.begin(); sIt != sendList.end(); ++sIt )
	{
    	if(((MessageData*)(&(*sIt)))->result_ethTag != NULL)
    		delete ((MessageData*)(&(*sIt)))->result_ethTag;
        if(((MessageData*)(&(*sIt)))->result_vlanTag != NULL)
            delete ((MessageData*)(&(*sIt)))->result_vlanTag;
        if(((MessageData*)(&(*sIt)))->result_hsrTag != NULL)
            delete ((MessageData*)(&(*sIt)))->result_hsrTag;
        if(((MessageData*)(&(*sIt)))->result_messageData != NULL)
            delete ((MessageData*)(&(*sIt)))->result_messageData;
	}

	for( rIt = recvList.begin(); rIt != recvList.end(); ++rIt )
	{
		numFramesReceivedDUP++;
    	if(((MessageData*)(&(*rIt)))->result_ethTag != NULL)
    		delete ((MessageData*)(&(*rIt)))->result_ethTag;
        if(((MessageData*)(&(*rIt)))->result_vlanTag != NULL)
            delete ((MessageData*)(&(*rIt)))->result_vlanTag;
        if(((MessageData*)(&(*rIt)))->result_hsrTag != NULL)
            delete ((MessageData*)(&(*rIt)))->result_hsrTag;
        if(((MessageData*)(&(*rIt)))->result_messageData != NULL)
            delete ((MessageData*)(&(*rIt)))->result_messageData;
	}

//	for( cIt = cpuList.begin(); cIt != cpuList.end(); ++cIt )
//	{
//    	if(((MessageData*)(&(*cIt)))->result_ethTag != NULL)
//    		delete ((MessageData*)(&(*cIt)))->result_ethTag;
//        if(((MessageData*)(&(*cIt)))->result_vlanTag != NULL)
//            delete ((MessageData*)(&(*cIt)))->result_vlanTag;
//        if(((MessageData*)(&(*cIt)))->result_hsrTag != NULL)
//            delete ((MessageData*)(&(*cIt)))->result_hsrTag;
//        if(((MessageData*)(&(*cIt)))->result_messageData != NULL)
//            delete ((MessageData*)(&(*cIt)))->result_messageData;
//	}

    recvList.clear();
    sendList.clear();
    cpuList.clear();

    recordScalar("frames that reach their destination",     numFramesReceivedOK);
   	recordScalar("frames that do not reach their destination",     numFramesReceivedFAIL);
    recordScalar("frames that reach their destination twice",     numFramesReceivedDUP);
}



