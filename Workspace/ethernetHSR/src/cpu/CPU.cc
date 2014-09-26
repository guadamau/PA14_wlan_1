#include "CPU.h"
#include "hsrDefines.h"
//#include "AddressTable.h"
#include "Ieee802Ctrl_m.h"
#include "MessagePacker.h"

Define_Module(CPU);

unsigned long CPU::messageCount = 0;

TestControl *
CPU::getTestControlInstance()
{
    cModule *calleeModule = getParentModule();

    while (calleeModule != NULL)
    {
        EV<< "getClassName():  " << calleeModule->getFullName() << " \n";
        if(calleeModule->getParentModule() != NULL)
        {
            calleeModule= calleeModule->getParentModule();
        }
        else
        {
            break;
        }
    }
    if(calleeModule != NULL)
    {
        calleeModule = calleeModule->getSubmodule("TestControl1"); //TODO: Namen als Parameter �bergeben
        return check_and_cast<TestControl *>(calleeModule);
    }

    return NULL;
}

EthernetIIFrame *
CPU::generateOnePacket(SendData sendData)
{

    EthernetIIFrame *result_ethTag = MessagePacker::createETHTag("eth", sendData.destination, myAddr);
    dataMessage *result_messageData =MessagePacker::createDataMessage("std", sendData.paketgroesse, messageCount++);
    vlanMessage *result_vlanTag = NULL;
    if (sendData.vlan != 0)
    {
        result_vlanTag = MessagePacker::createVLANTag( "vlan", 0, 0, 0, 0 );
    }
    EthernetIIFrame *result_ethFrame = MessagePacker::generateEthMessage(result_ethTag, result_vlanTag, NULL, result_messageData);
    MessagePacker::deleteMessage(&result_ethTag, &result_vlanTag, NULL, &result_messageData);

    return result_ethFrame;
}

void
CPU::printMessageinfo(SendData sendData)
{
    EV<< "-------------------------MESSAGE SEND-------------------------------- \n";
    EV << "destination " << sendData.destination << endl;
    switch(sendData.sendType)
    {
        case TYPE_HSR:
        {
            EV << "sendType TYPE_HSR"<< endl;
        }
        break;
        case TYPE_STD:
        {
            EV << "sendType TYPE_STD"<< endl;
        }
        break;
        default:
        {
            EV << "sendType TYPE_ERROR"<< endl;
        }
    }
    switch(sendData.sendBehavior)
    {
        case BEHAVIOR_CONSTANT_LOADGEN:
        {
            EV << "sendBehavior BEHAVIOR_CONSTANT_LOADGEN"<< endl;
        }
        break;
        case BEHAVIOR_UNIFORM_LOADGEN:
        {
            EV << "sendBehavior BEHAVIOR_UNIFORM_LOADGEN"<< endl;
        }
        break;
        case BEHAVIOR_NORMAL_LOADGEN:
        {
            EV << "sendBehavior BEHAVIOR_NORMAL_LOADGEN"<< endl;
        }
        break;
        case BEHAVIOR_STD:
        {
            EV << "sendBehavior BEHAVIOR_STD"<< endl;
        }
        break;
        default:
        {
            EV << "sendBehavior BEHAVIOR_ERROR"<< endl;
        }
    }
    EV << "startTime " << sendData.startTime << endl;
    EV << "stopTime " << sendData.stopTime << endl;
    EV << "last " << sendData.last << endl;
    EV << "maxlast " << sendData.maxLast << endl;
    EV << "interval " << sendData.interval << endl;
    EV << "maxinterval " << sendData.maxInterval << endl;
    EV << "standardabweichung " << sendData.standardabweichung << endl;
    EV << "paketgroesse " << sendData.paketgroesse << endl;
    EV << "simTime " << simTime() << endl;
    EV << "--------------------------------------------------------------------- \n";
}

bool
CPU::scheduleMessage(SendData sendData)
{
    //dataMessage *msg = NULL;
    MACAddress nullAddr("00-00-00-00-00-00");
    //Typ pr�fen
    if ((sendData.sendType != TYPE_STD) && (sendData.sendType != TYPE_HSR))
        return false;

    //Ziel pr�fen
    if (sendData.destination == nullAddr)
        return false;

    //Paketgr�sse pr�fen
    if (sendData.paketgroesse == 0)
        return false;

    //Rest ist abh�ngig vom verhalten
    switch (sendData.sendBehavior)
    {
    case BEHAVIOR_CONSTANT_LOADGEN:
    {
        if (sendData.startTime >= sendData.stopTime)
            return false;
        if ((sendData.last <= 0) && (sendData.interval <= 0))
            return false;
        cpuSelfMessage *delayedMessage = new cpuSelfMessage();
        delayedMessage->setSendData(sendData);
        scheduleAt(sendData.startTime, delayedMessage);
        return true;
    }
        break;
    case BEHAVIOR_UNIFORM_LOADGEN:
    {
        if (sendData.startTime >= sendData.stopTime)
            return false;
        if (((sendData.last > 0) && (sendData.maxLast > sendData.last)) || ((sendData.interval > 0) && (sendData.maxInterval > sendData.interval)))
        {
            cpuSelfMessage *delayedMessage = new cpuSelfMessage();
            delayedMessage->setSendData(sendData);
            scheduleAt(sendData.startTime, delayedMessage);
            return true;
        }
    }
        break;
    case BEHAVIOR_NORMAL_LOADGEN:
    {
        if (sendData.startTime >= sendData.stopTime)
            return false;
        if (sendData.standardabweichung <= 0)
            return false;
        if ((sendData.last > 0) || (sendData.interval > 0))
        {
            cpuSelfMessage *delayedMessage = new cpuSelfMessage();
            delayedMessage->setSendData(sendData);
            scheduleAt(sendData.startTime, delayedMessage);
            return true;
        }
    }
        break;
    case BEHAVIOR_STD:
    {
        cpuSelfMessage *delayedMessage = new cpuSelfMessage();
        delayedMessage->setSendData(sendData);
        scheduleAt(sendData.startTime, delayedMessage);

        return true;
    }
        break;
    default:
    {
        throw cRuntimeError("CPU undefinierter zustand ! Panik ! \n");
        //leerer defaultblock um warnungen zu verhindern
    }
    }

    return false;
}

void
CPU::loadXMLFile()
{
    EV<< " Start XML parser \n";
    int counter;
    char tagTemp[18];
    char srcTemp[18];
    char *attributeTemp = NULL;
    SendData sendData;
    cXMLElement *valueElement = NULL;
    cXMLElement *paketElement = NULL;
    cXMLElement *sourceElement = NULL;

    MACAddress nullAddr("00-00-00-00-00-00");

    snprintf(tagTemp, 18, "%02x-%02x-%02x-%02x-%02x-%02x", myAddr.getAddressByte(0), myAddr.getAddressByte(1), myAddr.getAddressByte(2), myAddr.getAddressByte(3), myAddr.getAddressByte(4), myAddr.getAddressByte(5));
    sourceElement = rootelement->getFirstChildWithTag("source");
    while(sourceElement != NULL)
    {
    	memcpy(srcTemp,sourceElement->getNodeValue(),17);
    	for(counter = 0;counter < 17; counter++)
    	{
    		if(srcTemp[counter] == '?' )
    		{
    			srcTemp[counter] = tagTemp[counter];
    		}
    	}
        //EV << "\n ELE --"<< " " << "-- Gesucht: " << tagTemp << " " <<  " \n";
        if(strncasecmp(tagTemp,srcTemp,17) == 0)
        {
            //EV << "PASST \n";
            paketElement = sourceElement->getFirstChildWithTag("lastmuster");
            while(paketElement != NULL)
            {
                sendData.destination = nullAddr;
                sendData.sendType = TYPE_ERROR;
                sendData.sendBehavior = BEHAVIOR_STD;
                sendData.startTime = 0;
                sendData.stopTime = 0;
                sendData.last = 0;
                sendData.maxLast = 0;
                sendData.interval = 0;
                sendData.maxInterval = 0;
                sendData.standardabweichung = 0;
                sendData.paketgroesse = 0;
                sendData.vlan = 0;

                ///////////////////////////////////////// destination ///////////////////////////////////////////////////////////////
                valueElement = paketElement->getFirstChildWithTag("destination");
                if(valueElement != NULL)
                {
                    attributeTemp = (char*)valueElement->getNodeValue();
                    if(strncasecmp("BROADCASTADDR",attributeTemp,13) == 0)
                    {
                        sendData.destination.setAddress("FF-FF-FF-FF-FF-FF");//= BROADCASTADDR;
                    }
                    else
                    {
                        sendData.destination.setAddress(attributeTemp);
                    }
                }
                ///////////////////////////////////////// TYP ///////////////////////////////////////////////////////////////
                valueElement = paketElement->getFirstChildWithTag("typ");
                if(valueElement != NULL)
                {
                    attributeTemp = (char*)valueElement->getNodeValue();
                    if (strncasecmp("HSR",attributeTemp,3) == 0)
                    {
                        sendData.sendType = TYPE_HSR;
                    }
                    else if (strncasecmp("STD",attributeTemp,3) == 0)
                    {
                        sendData.sendType = TYPE_STD;
                    }
                    else
                    {
                        sendData.sendType = TYPE_ERROR;
                    }
                }
                ///////////////////////////////////////// verhalten ///////////////////////////////////////////////////////////////
                valueElement = paketElement->getFirstChildWithTag("verhalten");
                if(valueElement != NULL)
                {
                    attributeTemp = (char*)valueElement->getNodeValue();
                    if (strncasecmp("einmalig",attributeTemp,8) == 0)
                    {
                        sendData.sendBehavior = BEHAVIOR_STD;
                    }
                    else if (strncasecmp("konstante last",attributeTemp,14) == 0)
                    {
                        sendData.sendBehavior = BEHAVIOR_CONSTANT_LOADGEN;
                    }
                    else if (strncasecmp("gleichverteilte last",attributeTemp,20) == 0)
                    {
                        sendData.sendBehavior = BEHAVIOR_UNIFORM_LOADGEN;
                    }
                    else if (strncasecmp("normalverteilte last",attributeTemp,19) == 0)
                    {
                        sendData.sendBehavior = BEHAVIOR_NORMAL_LOADGEN;
                    }
                    else
                    {
                        sendData.sendBehavior = BEHAVIOR_ERROR;
                    }
                }
                ///////////////////////////////////////// startzeit ///////////////////////////////////////////////////////////////
                valueElement = paketElement->getFirstChildWithTag("startzeit");
                if(valueElement != NULL)
                {
                    attributeTemp = (char*)valueElement->getNodeValue();
                    sendData.startTime = atof(attributeTemp);
                }
                ///////////////////////////////////////// stopzeit ///////////////////////////////////////////////////////////////
                valueElement = paketElement->getFirstChildWithTag("stopzeit");
                if(valueElement != NULL)
                {
                    attributeTemp = (char*)valueElement->getNodeValue();
                    sendData.stopTime = atof(attributeTemp);
                }

                ///////////////////////////////////////// last ///////////////////////////////////////////////////////////////
                valueElement = paketElement->getFirstChildWithTag("last");
                if(valueElement != NULL)
                {
                    attributeTemp = (char*)valueElement->getNodeValue();
                    sendData.last = atof(attributeTemp);
                }
                ///////////////////////////////////////// maxlast ///////////////////////////////////////////////////////////////
                valueElement = paketElement->getFirstChildWithTag("maxlast");
                if(valueElement != NULL)
                {
                    attributeTemp = (char*)valueElement->getNodeValue();
                    sendData.maxLast = atof(attributeTemp);
                }
                ///////////////////////////////////////// interval ///////////////////////////////////////////////////////////////
                valueElement = paketElement->getFirstChildWithTag("interval");
                if(valueElement != NULL)
                {
                    attributeTemp = (char*)valueElement->getNodeValue();
                    sendData.interval = atof(attributeTemp);
                }
                ///////////////////////////////////////// maxinterval ///////////////////////////////////////////////////////////////
                valueElement = paketElement->getFirstChildWithTag("maxinterval");
                if(valueElement != NULL)
                {
                    attributeTemp = (char*)valueElement->getNodeValue();
                    sendData.maxInterval = atof(attributeTemp);
                }
                ///////////////////////////////////////// standardabweichung ///////////////////////////////////////////////////////////////
                valueElement = paketElement->getFirstChildWithTag("standardabweichung");
                if(valueElement != NULL)
                {
                    attributeTemp = (char*)valueElement->getNodeValue();
                    sendData.standardabweichung = atof(attributeTemp);
                }
                ///////////////////////////////////////// paketgroesse ///////////////////////////////////////////////////////////////
                valueElement = paketElement->getFirstChildWithTag("paketgroesse");
                if(valueElement != NULL)
                {
                    attributeTemp = (char*)valueElement->getNodeValue();
                    sendData.paketgroesse = atoi(attributeTemp);
                }
                ///////////////////////////////////////// vlan ///////////////////////////////////////////////////////////////
                valueElement = paketElement->getFirstChildWithTag("vlan");
                if(valueElement != NULL)
                {
                    attributeTemp = (char*)valueElement->getNodeValue();
                    sendData.vlan = atoi(attributeTemp);
                }

                if(sendData.destination != myAddr)
                {
                    if(scheduleMessage(sendData) == false)
                    {
                        throw cRuntimeError("Illegal XML configuration");
                    }
                }
                paketElement = paketElement->getNextSibling();
            }
        }

        sourceElement = sourceElement->getNextSibling();
    }
}

void
CPU::initialize()
{
    testControl = getTestControlInstance();
    if (testControl == NULL)
    {
        throw cRuntimeError("can not get TestControlInstance");
    }
    
    numFramesSent = 0;
    numFramesReceived = 0;      
    
    WATCH(numFramesSent);
    WATCH(numFramesReceived);    

    //"01-to-01 Delay"  //15chars
    /*
    char temp[15];
    memcpy(temp,"01-to-01 Delay",14);
    temp[14] = 0;
    temp[6] = par("myAddress").stringValue()[15];
    temp[7] = par("myAddress").stringValue()[16];

    temp[1] = '1';
    endToEndDelayVec[0].setName(temp);
    temp[1] = '2';
    endToEndDelayVec[1].setName(temp);
    temp[1] = '3';
    endToEndDelayVec[2].setName(temp);
    temp[1] = '4';
    endToEndDelayVec[3].setName(temp);
    temp[1] = '5';
    endToEndDelayVec[4].setName(temp);
    temp[1] = '6';
    endToEndDelayVec[5].setName(temp);
    temp[1] = '7';
    endToEndDelayVec[6].setName(temp);
    temp[1] = '8';
    endToEndDelayVec[7].setName(temp);
    temp[1] = '9';
    endToEndDelayVec[8].setName(temp);
    temp[0] = '1';
    temp[1] = '0';
    endToEndDelayVec[9].setName(temp);
*/
    myAddr.setAddress(par("myAddress").stringValue());

    if ((myAddr.isBroadcast()) || (myAddr.isMulticast()))
    {
        throw cRuntimeError("invalid Addr");
    }

    gateIn = gate("gate$i");
    gateOut = gate("gate$o");

    rootelement = par("xmlparam").xmlValue();

    if (testControl->registerCPU(myAddr) == false)
    {
        throw cRuntimeError("CPU Adresskonflikt ! Panik ! \n");
    }

    loadXMLFile();
}

void
CPU::handleMessage(cMessage *msg)
{

    if (msg->isSelfMessage())
    {
        cpuSelfMessage *delayedMessage = check_and_cast<cpuSelfMessage *> (msg);
        if ((simTime() < delayedMessage->getSendData().stopTime) && (delayedMessage->getSendData().sendBehavior != BEHAVIOR_STD))
        {

            double randomLast = 0;
            double randomInterval = 0;
            //EV << " WTF: " << delayedMessage->getSendData().sendBehavior << "\n";
            switch (delayedMessage->getSendData().sendBehavior)
            {
				case BEHAVIOR_CONSTANT_LOADGEN:
				{
					if (delayedMessage->getSendData().last == 0)
					{
						randomInterval = delayedMessage->getSendData().interval;
					}
					else
					{
						randomLast = delayedMessage->getSendData().last;
					}
				}
					break;
				case BEHAVIOR_UNIFORM_LOADGEN:
				{
					if (delayedMessage->getSendData().last == 0)
					{
						randomInterval = uniform(delayedMessage->getSendData().interval, delayedMessage->getSendData().maxInterval);
					}
					else
					{
						randomLast = uniform(delayedMessage->getSendData().last, delayedMessage->getSendData().maxLast);
					}
				}
					break;
				case BEHAVIOR_NORMAL_LOADGEN:
				{
					if (delayedMessage->getSendData().last == 0)
					{
						randomInterval = truncnormal(delayedMessage->getSendData().interval, delayedMessage->getSendData().standardabweichung);
					}
					else
					{
						randomLast = truncnormal(delayedMessage->getSendData().last, delayedMessage->getSendData().standardabweichung);
					}
				}
					break;
				default:
				{
					throw cRuntimeError("Interner Fehler in CPU::handleMessage  ! \n");
				}
            }

            simtime_t sendTime;
            if (randomLast == 0)
            {
                sendTime = randomInterval + simTime();
            }
            else
            {

                double byteProSec = randomLast * 1024.0 * 1024.0 / 8.0; //bytes pro sec berechnen
                //EV << "byteProSec "<< byteProSec <<" \n";
                double anzahlPaketeProSec = byteProSec / (delayedMessage->getSendData().paketgroesse + 32); //Die 32 extrabytes die sp�ter hinzukommen bestehen aus : 2 EtherType + 4CRC + 6 Source + 6 Destination + 4 HSR Tag + 2HSR Ethertype +1 SFD + 7 Pr�ambel
                //EV << "anzahlPaketeProSec "<< anzahlPaketeProSec <<" \n";
                double interval = 1.0 / anzahlPaketeProSec;
                //EV << "interval "<< interval <<" \n";
                sendTime = interval + simTime();
            }

            scheduleAt(sendTime, delayedMessage);

            EthernetIIFrame *dmsg = generateOnePacket(delayedMessage->getSendData());
            testControl->registerSEND(myAddr, dmsg->dup(), simTime());
            numFramesSent++;
            send(dmsg, gateOut);
        }
        else
        {
            if(delayedMessage->getSendData().sendBehavior == BEHAVIOR_STD)
            {
                EthernetIIFrame *outmsg = generateOnePacket(delayedMessage->getSendData());
                testControl->registerSEND(myAddr, outmsg->dup(), delayedMessage->getSendData().startTime);
                numFramesSent++;
                send(outmsg, gateOut);
            }
            delete msg;
        }
    }
    else
    {
        EthernetIIFrame *packet = check_and_cast<EthernetIIFrame *>(msg);
        numFramesReceived++;
        if((packet->getDest().isBroadcast() == false) && (packet->getDest().isMulticast() == false) && (packet->getDest() != myAddr))
        {
            EV << "CPU: " << myAddr << " Missroutet Message ARRIVED! Gate: " << msg->getArrivalGate()->getBaseName() << " \n";
        }

        testControl->registerRECV(myAddr, packet->dup(), simTime());

        vlanMessage *vlanTag = NULL;
        dataMessage *messageData = NULL;
        hsrMessage *hsrTag = NULL;
        MessagePacker::decapsulateMessage(&packet, &vlanTag, &hsrTag , &messageData);


        //result_messageData->setByteLength(sendData.paketgroesse);
        /*
        if(packet->getSrc().equals(MACAddress("00-15-12-14-88-01")))
            endToEndDelayVec[0].record(simTime() - messageData->getSendTime());
        if(packet->getSrc().equals(MACAddress("00-15-12-14-88-02")))
            endToEndDelayVec[1].record(simTime() - messageData->getSendTime());
        if(packet->getSrc().equals(MACAddress("00-15-12-14-88-03")))
            endToEndDelayVec[2].record(simTime() - messageData->getSendTime());
        if(packet->getSrc().equals(MACAddress("00-15-12-14-88-04")))
            endToEndDelayVec[3].record(simTime() - messageData->getSendTime());
        if(packet->getSrc().equals(MACAddress("00-15-12-14-88-05")))
            endToEndDelayVec[4].record(simTime() - messageData->getSendTime());
        if(packet->getSrc().equals(MACAddress("00-15-12-14-88-06")))
            endToEndDelayVec[5].record(simTime() - messageData->getSendTime());
        if(packet->getSrc().equals(MACAddress("00-15-12-14-88-07")))
            endToEndDelayVec[6].record(simTime() - messageData->getSendTime());
        if(packet->getSrc().equals(MACAddress("00-15-12-14-88-08")))
            endToEndDelayVec[7].record(simTime() - messageData->getSendTime());
        if(packet->getSrc().equals(MACAddress("00-15-12-14-88-09")))
            endToEndDelayVec[8].record(simTime() - messageData->getSendTime());
        if(packet->getSrc().equals(MACAddress("00-15-12-14-88-10")))
            endToEndDelayVec[9].record(simTime() - messageData->getSendTime());
    */

        //EV << "CPU: " << myAddr << " Multi- or Broadcast Message ARRIVED! Gate: " << msg->getArrivalGate()->getBaseName() << " \n";
        MessagePacker::deleteMessage(&packet, &vlanTag, &hsrTag , &messageData);

//        if(packet != NULL)
//            delete packet;
//        if(vlanTag != NULL)
//            delete vlanTag;
//        if(messageData != NULL)
//            delete messageData;
    }
}

void
CPU::finish()
{
    
    recordScalar("cpu frames sent",    numFramesSent);
    recordScalar("cpu frames rcvd",    numFramesReceived);
    //testControl->printStat();
}

CPU::CPU()
{
}
CPU::~CPU()
{
    //TestControl::delInstance();
    testControl = NULL;
}
