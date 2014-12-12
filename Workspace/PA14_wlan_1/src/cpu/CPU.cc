#include "CPU.h"
#include "hsrDefines.h"
//#include "AddressTable.h"
#include "Ieee802Ctrl_m.h"
#include "MessagePacker.h"

Define_Module(CPU);

unsigned long CPU::messageCount = 0;

EthernetIIFrame *
CPU::generateOnePacket(SendData sendData)
{
    const char * framePrioArr[] = {"EXPRESS", "HIGH", "LOW"};

    /*
    if (sendData.frameprio == EXPRESS)
    {
        result_ethTag->setEtherType(0x8500);
    }
    */

    EthernetIIFrame *result_ethTag = NULL;

    const char* ethFrameName = "Uni";

    if(sendData.destination.isBroadcast()) {
        ethFrameName = "Broad";
    }
    else if(sendData.destination.isMulticast()) {
        ethFrameName = "Multi";
    }

    /*
     * Display Seq# in GUI
     */

    std::stringstream ss;
    ss << ethFrameName << " From: " << getParentModule()->getFullName() << " (" << framePrioArr[sendData.frameprio] <<") Seq#" << sequenceNumber;
    ethFrameName = ss.str().c_str();

    result_ethTag = MessagePacker::createETHTag( ethFrameName, sendData.destination, macAddress );

    hsrMessage *result_hsrTag = NULL;
    result_hsrTag = MessagePacker::createHSRTag( "HSR", 1, sequenceNumber );

    vlanMessage *result_vlanTag = NULL;
    result_vlanTag = MessagePacker::createVLANTag( "vlan", sendData.frameprio, 0, 0, 0 );

    dataMessage* result_dataMessage = NULL;
    result_dataMessage = MessagePacker::createDataMessage( "stdData", sendData.paketgroesse, messageCount++ );

    EthernetIIFrame* result_ethFrame = NULL;
    result_ethFrame = MessagePacker::generateEthMessage( result_ethTag, result_vlanTag, result_hsrTag, result_dataMessage );


    MessagePacker::deleteMessage(&result_ethTag, &result_vlanTag, &result_hsrTag, &result_dataMessage);

    EV << "[ OK ] " << sendData.frameprio << " Message created at: " << result_ethFrame->getCreationTime() << " created." << "  |  Seq#: " << sequenceNumber << endl;
    sequenceNumber++;

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
    EV << "frameprio " << sendData.frameprio << endl;
    EV << "simTime " << simTime() << endl;
    EV << "--------------------------------------------------------------------- \n";
}

bool
CPU::scheduleMessage(SendData sendData)
{
    MACAddress nullAddr("00-00-00-00-00-00");

    if (((sendData.sendType != TYPE_STD) && (sendData.sendType != TYPE_HSR)) || sendData.destination == nullAddr || sendData.paketgroesse == 0)
    {
        return false;
    }

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

    snprintf(tagTemp, 18, "%02x-%02x-%02x-%02x-%02x-%02x", macAddress.getAddressByte(0), macAddress.getAddressByte(1), macAddress.getAddressByte(2), macAddress.getAddressByte(3), macAddress.getAddressByte(4), macAddress.getAddressByte(5));
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
                sendData.epsilon = 0.0;
                sendData.maxInterval = 0;
                sendData.standardabweichung = 0;
                sendData.paketgroesse = 0;
                sendData.vlan = 0;
                sendData.frameprio = LOW;

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

                valueElement = paketElement->getFirstChildWithTag("startzeit");
                if(valueElement != NULL)
                {
                    attributeTemp = (char*)valueElement->getNodeValue();
                    sendData.startTime = atof(attributeTemp);
                }

                valueElement = paketElement->getFirstChildWithTag("stopzeit");
                if(valueElement != NULL)
                {
                    attributeTemp = (char*)valueElement->getNodeValue();
                    sendData.stopTime = atof(attributeTemp);
                }

                valueElement = paketElement->getFirstChildWithTag("last");
                if(valueElement != NULL)
                {
                    attributeTemp = (char*)valueElement->getNodeValue();
                    sendData.last = atof(attributeTemp);
                }

                valueElement = paketElement->getFirstChildWithTag("maxlast");
                if(valueElement != NULL)
                {
                    attributeTemp = (char*)valueElement->getNodeValue();
                    sendData.maxLast = atof(attributeTemp);
                }

                valueElement = paketElement->getFirstChildWithTag("interval");
                if(valueElement != NULL)
                {
                    attributeTemp = (char*)valueElement->getNodeValue();
                    sendData.interval = atof(attributeTemp);
                }

                valueElement = paketElement->getFirstChildWithTag( "epsilon" );
                if( valueElement != NULL )
                {
                    attributeTemp = ( char* )valueElement->getNodeValue();
                    sendData.epsilon = atof( attributeTemp );
                }

                valueElement = paketElement->getFirstChildWithTag("maxinterval");
                if(valueElement != NULL)
                {
                    attributeTemp = (char*)valueElement->getNodeValue();
                    sendData.maxInterval = atof(attributeTemp);
                }

                valueElement = paketElement->getFirstChildWithTag("standardabweichung");
                if(valueElement != NULL)
                {
                    attributeTemp = (char*)valueElement->getNodeValue();
                    sendData.standardabweichung = atof(attributeTemp);
                }

                valueElement = paketElement->getFirstChildWithTag("paketgroesse");
                if(valueElement != NULL)
                {
                    attributeTemp = (char*)valueElement->getNodeValue();
                    sendData.paketgroesse = atoi(attributeTemp);
                }

                valueElement = paketElement->getFirstChildWithTag("vlan");
                if(valueElement != NULL)
                {
                    attributeTemp = (char*)valueElement->getNodeValue();
                    sendData.vlan = atoi(attributeTemp);
                }

                valueElement = paketElement->getFirstChildWithTag("priority");
                if(valueElement != NULL)
                {
                    attributeTemp = (char*)valueElement->getNodeValue();
                    if (strncasecmp("EXPRESS",attributeTemp,7) == 0)
                    {
                        sendData.frameprio = EXPRESS;
                    }
                    else if (strncasecmp("HIGH",attributeTemp,4) == 0)
                    {
                        sendData.frameprio = HIGH;
                    }
                    else
                    {
                        sendData.frameprio = LOW;
                    }
                }

                if(sendData.destination != macAddress)
                {
                    EV << "Scheduling senddata";
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
    sequenceNumber = 0;
    
    numFramesSent = 0;
    numFramesReceived = 0;      
    
    WATCH(numFramesSent);
    WATCH(numFramesReceived);    

    macAddress.setAddress(par("macAddress").stringValue());

    if ((macAddress.isBroadcast()) || (macAddress.isMulticast()))
    {
        throw cRuntimeError("invalid Addr");
    }

    gateIn = gate( "gate$i" );
    gateInExp = gate( "gateExp$i" );
    gateOut = gate( "gate$o" );
    gateOutExp = gate( "gateExp$o" );

    rootelement = par("xmlparam").xmlValue();

    multicastListener = par("multicastListener");

    loadXMLFile();

    /* There must be a delay logger on top of the current simulation.
     * Otherwise the following lines will result in an error. */

    if( getModuleByPath( "HsrNetworkSimple.delayLogger" ) != NULL )
    {
        this->delayLogger = check_and_cast<DelayLogger*>( getModuleByPath( "HsrNetworkSimple.delayLogger" ) );
    }
    else if( getModuleByPath( "SubstationHSR8mu2pu.delayLogger" ) != NULL )
    {
        this->delayLogger = check_and_cast<DelayLogger*>( getModuleByPath( "SubstationHSR8mu2pu.delayLogger" ) );
    }
    else if( getModuleByPath( "SubstationHSR17mu2pu.delayLogger" ) != NULL )
    {
        this->delayLogger = check_and_cast<DelayLogger*>( getModuleByPath( "SubstationHSR17mu2pu.delayLogger" ) );
    }
    else
    {
        throw cRuntimeError( "No delayLogger found. Is the name of the delayLogger set correct?" );
        endSimulation();
    }
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
            double interval = 0.0;
            double epsilon = 0.0;

            switch (delayedMessage->getSendData().sendBehavior)
            {
				case BEHAVIOR_CONSTANT_LOADGEN:
				{
					if (delayedMessage->getSendData().last == 0)
					{
						interval = delayedMessage->getSendData().interval;

						if( delayedMessage->getSendData().epsilon > 0.0 )
						{
                            epsilon = delayedMessage->getSendData().epsilon;
                            interval = uniform( interval, ( interval + epsilon ) );
                            /* next line just for console output */
                            epsilon = interval - delayedMessage->getSendData().interval;
						}
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
						interval = uniform(delayedMessage->getSendData().interval, delayedMessage->getSendData().maxInterval);
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
						interval = truncnormal(delayedMessage->getSendData().interval, delayedMessage->getSendData().standardabweichung);
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
                EV << "Random generated epsilon: " << epsilon << endl;
                EV << "Interval for sending: " << interval << endl;
                sendTime = interval + simTime();
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
            numFramesSent++;

            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            /* Ethertype 0x8500 is an express frame. */
            if( dmsg->getEtherType() == 0x8500 )
            {
                /* Use the express channel. */
                send( dmsg, gateOutExp );
            }
            else
            {
                /* Use the normal channel. */
                send( dmsg, gateOut );
            }

        }
        else
        {
            if(delayedMessage->getSendData().sendBehavior == BEHAVIOR_STD)
            {
                EthernetIIFrame *outmsg = generateOnePacket(delayedMessage->getSendData());
                numFramesSent++;

                /* Ethertype 0x8500 is an express frame. */
                if( outmsg->getEtherType() == 0x8500 )
                {
                    /* Use the express channel. */
                    send( outmsg, gateOutExp );
                }
                else
                {
                    /* Use the normal channel. */
                    send( outmsg, gateOut );
                }
            }
            delete msg;
        }
    }
    else
    {
        cMessage* frameForDelayLogger = msg->dup();
        EthernetIIFrame *packet = check_and_cast<EthernetIIFrame *>(msg);

        std::stringstream ss;
        ss << "Recieved " << packet->getName();

        numFramesReceived++;
        if((packet->getDest().isBroadcast() == false) && (packet->getDest().isMulticast() == false) && (packet->getDest() != macAddress))
        {
            EV << "CPU: " << macAddress << " Missrouted Message ARRIVED! Gate: " << msg->getArrivalGate()->getBaseName() << " \n";
        }
        else if( packet->getDest() == macAddress ) {
            getParentModule()->bubble( ss.str().c_str() );
        }

        vlanMessage *vlanTag = NULL;
        dataMessage *messageData = NULL;
        hsrMessage *hsrTag = NULL;
        MessagePacker::decapsulateMessage(&packet, &vlanTag, &hsrTag , &messageData);


        if(multicastListener == 1)
        {
            getParentModule()->bubble( ss.str().c_str() );
        }

        /* Log circulation multicast frames */
        if( macAddress == packet->getSrc() &&
            packet->getDest().isMulticast() )
        {
            this->delayLogger->addDelay( frameForDelayLogger );
        }
        /* Log also unicast frames to me ... */
        else if( macAddress == packet->getDest() )
        {
            this->delayLogger->addDelay( frameForDelayLogger );
        }
        /* Log circulating unicast or undefined frames */
        else if( macAddress == packet->getSrc() &&
                 !packet->getDest().isMulticast() &&
                 !packet->getDest().isBroadcast() )
        {
            this->delayLogger->addDelay( frameForDelayLogger );
        }

        MessagePacker::deleteMessage( &packet, &vlanTag, &hsrTag , &messageData );

    }
}

void
CPU::finish()
{
    recordScalar("cpu frames sent",    numFramesSent);
    recordScalar("cpu frames rcvd",    numFramesReceived);
}

CPU::CPU()
{
}
CPU::~CPU()
{
}
