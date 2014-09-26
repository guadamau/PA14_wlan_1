//
// Copyright (C) 2006 Levente Meszaros
// Copyright (C) 2004 Andras Varga
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//

#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include "ModifiedEtherMACBase.h"
#include "IPassiveQueue.h"
#include "IInterfaceTable.h"
#include "InterfaceTableAccess.h"
#include "MessagePacker.h"

// Constants from the 802.3 spec
#define MAX_PACKETBURST              13
#define GIGABIT_MAX_BURST_BYTES      8192  /* don't start new frame after 8192 or more bytes already transmitted */
#define MAX_ETHERNET_DATA            1500  /* including LLC, SNAP etc headers */
#define MAX_ETHERNET_FRAME           1518  /* excludes preamble and SFD */
#define MIN_ETHERNET_FRAME           64    /* excludes preamble and SFD */
#define GIGABIT_MIN_FRAME_WITH_EXT   512 /* excludes preamble and SFD, but includes 448 byte extension */
#define INTERFRAME_GAP_BITS          96
#define ETHERNET_TXRATE              10000000.0    /* 10 Mbit/sec (in bit/s) */
#define FAST_ETHERNET_TXRATE         100000000.0   /* 100 Mbit/sec (in bit/s) */
#define GIGABIT_ETHERNET_TXRATE      1000000000.0  /* 1 Gbit/sec (in bit/s) */
#define FAST_GIGABIT_ETHERNET_TXRATE 10000000000.0 /* 10 Gbit/sec (in bit/s) */
#define SLOT_TIME                    (512.0/ETHERNET_TXRATE)  /* for Ethernet & Fast Ethernet, in seconds */
#define GIGABIT_SLOT_TIME            (4096.0/GIGABIT_ETHERNET_TXRATE) /* seconds */
#define MAX_ATTEMPTS                 16
#define BACKOFF_RANGE_LIMIT          10
#define JAM_SIGNAL_BYTES             4
#define PREAMBLE_BYTES               7
#define SFD_BYTES                    1
#define PAUSE_BITTIME                512 /* pause is in 512-bit-time units */

Define_Module(ModifiedEtherMACBase);

static const double MODIFIED_SPEED_OF_LIGHT = 200000000.0;


ModifiedEtherMACBase::ModifiedEtherMACBase()
{
    nb = NULL;
    interfaceEntry = NULL;
    endTxMsg = endIFGMsg = NULL;
}

ModifiedEtherMACBase::~ModifiedEtherMACBase()
{
    cancelAndDelete(endTxMsg);
    cancelAndDelete(endIFGMsg);
}

void ModifiedEtherMACBase::initialize()
{
    physOutGate = gate("phys$o");

    initializeFlags();

    //initializeTxrate();
    txrate = 100000000; //100Mbps
    WATCH(txrate);

//    initializeMACAddress();

    const char *addrstr = par("address");
    address.setAddress(addrstr);

//  duplicate detection
    cModule *calleeModule = getParentModule();
    const char *duplicateDetectionString = par("duplicateDetection");
    calleeModule = calleeModule->getSubmodule(duplicateDetectionString); 
    duplicateDetection = check_and_cast<ListenErkennung *>(calleeModule);    
    
    //return NULL;

//    initializeNotificationBoard();
    initializeStatistics();

//    registerInterface(txrate); // needs MAC address

    // initialize queue
    txQueue.setName("txQueue");

    // initialize self messages
    endTxMsg = new cMessage("EndTransmission", ENDTRANSMISSION);
    endIFGMsg = new cMessage("EndIFG", ENDIFG);

    // initialize states
    transmitState = TX_IDLE_STATE;
    receiveState = RX_IDLE_STATE;
    WATCH(transmitState);
    WATCH(receiveState);

    // initialize queue limit
    txQueueLimit = par("txQueueLimit");
    WATCH(txQueueLimit);

    duplexMode = true;
    calculateParameters();

    beginSendFrames();
}


// void ModifiedEtherMACBase::initializeMACAddress()
// {
//     const char *addrstr = par("address");

//     if (!strcmp(addrstr,"auto"))
//     {
//         // assign automatic address
//         address = MACAddress::generateAutoAddress();

//         // change module parameter from "auto" to concrete address
//         par("address").setStringValue(address.str().c_str());
//     }
//     else
//     {
//         address.setAddress(addrstr);
//     }
// }

// void ModifiedEtherMACBase::initializeNotificationBoard()
// {
// //     hasSubscribers = false;
// //     if (interfaceEntry) {
// //         nb = NotificationBoardAccess().getIfExists();
// //         notifDetails.setInterfaceEntry(interfaceEntry);
// //         nb->subscribe(this, NF_SUBSCRIBERLIST_CHANGED);
// //         //updateHasSubcribers();
// //     }
// }

void ModifiedEtherMACBase::initializeFlags()
{
    // initialize connected flag
    connected = physOutGate->getPathEndGate()->isConnected();
    if (!connected)
        EV << "MAC not connected to a network.\n";
    WATCH(connected);

    // TODO: this should be settable from the gui
    // initialize disabled flag
    // Note: it is currently not supported to enable a disabled MAC at runtime.
    // Difficulties: (1) autoconfig (2) how to pick up channel state (free, tx, collision etc)
    disabled = false;
    WATCH(disabled);

    // initialize promiscuous flag
    promiscuous = par("promiscuous");
    WATCH(promiscuous);
}

void ModifiedEtherMACBase::initializeStatistics()
{
    framesSentInBurst = 0;
    bytesSentInBurst = 0;
    numFramesDroppedQueueFull = 0;

    numFramesSent = numFramesReceivedOK = numBytesSent = numBytesReceivedOK = 0;
    numFramesPassedToHL = numDroppedBitError = numDroppedNotForUs = 0;
    numFramesFromHL = numDroppedIfaceDown = 0;

    WATCH(framesSentInBurst);
    WATCH(bytesSentInBurst);

    WATCH(numFramesSent);
    WATCH(numFramesReceivedOK);
    WATCH(numBytesSent);
    WATCH(numBytesReceivedOK);
    WATCH(numFramesFromHL);
    WATCH(numDroppedIfaceDown);
    WATCH(numDroppedBitError);
    WATCH(numDroppedNotForUs);
    WATCH(numFramesPassedToHL);
    WATCH(numFramesDroppedQueueFull);

    numFramesSentVector.setName("framesSent");
    numFramesReceivedOKVector.setName("framesReceivedOK");
    numBytesSentVector.setName("bytesSent");
    numBytesReceivedOKVector.setName("bytesReceivedOK");
    numDroppedIfaceDownVector.setName("framesDroppedIfaceDown");
    numDroppedBitErrorVector.setName("framesDroppedBitError");
    numDroppedNotForUsVector.setName("framesDroppedNotForUs");
    numFramesPassedToHLVector.setName("framesPassedToHL");
    queueLengthVector.setName("queue length");
    numDroppedQueueFullVector.setName("framesDroppedQueueFull");
}

// void ModifiedEtherMACBase::registerInterface(double txrate)
// {
//     IInterfaceTable *ift = InterfaceTableAccess().getIfExists();
//     if (!ift)
//         return;

//     interfaceEntry = new InterfaceEntry();

//     // interface name: our module name without special characters ([])
//     char *interfaceName = new char[strlen(getParentModule()->getFullName())+1];
//     char *d=interfaceName;
//     for (const char *s=getParentModule()->getFullName(); *s; s++)
//         if (isalnum(*s))
//             *d++ = *s;
//     *d = '\0';

//     interfaceEntry->setName(interfaceName);
//     delete [] interfaceName;

//     // data rate
//     interfaceEntry->setDatarate(txrate);

//     // generate a link-layer address to be used as interface token for IPv6
//     interfaceEntry->setMACAddress(address);
//     interfaceEntry->setInterfaceToken(address.formInterfaceIdentifier());
//     //InterfaceToken token(0, simulation.getUniqueNumber(), 64);
//     //interfaceEntry->setInterfaceToken(token);

//     // MTU: typical values are 576 (Internet de facto), 1500 (Ethernet-friendly),
//     // 4000 (on some point-to-point links), 4470 (Cisco routers default, FDDI compatible)
//     interfaceEntry->setMtu(par("mtu"));

//     // capabilities
//     interfaceEntry->setMulticast(true);
//     interfaceEntry->setBroadcast(true);

//     // add
//     ift->addInterface(interfaceEntry, this);
// }


bool ModifiedEtherMACBase::checkDestinationAddress(EtherFrame *frame)
{
    // If not set to promiscuous = on, then checks if received frame contains destination MAC address
    // matching port's MAC address, also checks if broadcast bit is set
    if (!promiscuous && !frame->getDest().isBroadcast() && !frame->getDest().equals(address))
    {
        EV << "Frame `" << frame->getName() <<"' not destined to us, discarding\n";
        numDroppedNotForUs++;
        numDroppedNotForUsVector.record(numDroppedNotForUs);
    	EthernetIIFrame *packet = check_and_cast<EthernetIIFrame *>(frame);
        vlanMessage *vlanTag = NULL;
        dataMessage *messageData = NULL;
        hsrMessage *hsrTag = NULL;
        MessagePacker::decapsulateMessage(&packet, &vlanTag, &hsrTag , &messageData);
        MessagePacker::deleteMessage(&packet, &vlanTag, &hsrTag , &messageData);
        frame = NULL;

        return false;
    }

    return true;
}

void ModifiedEtherMACBase::calculateParameters()
{
    if (disabled || !connected)
    {
        bitTime = slotTime = interFrameGap = shortestFrameDuration = 0;
        carrierExtension = frameBursting = false;
        return;
    }

    if (txrate != ETHERNET_TXRATE && txrate != FAST_ETHERNET_TXRATE &&
        txrate != GIGABIT_ETHERNET_TXRATE && txrate != FAST_GIGABIT_ETHERNET_TXRATE)
    {
        error("nonstandard transmission rate %g, must be %g, %g, %g or %g bit/sec",
            txrate, ETHERNET_TXRATE, FAST_ETHERNET_TXRATE, GIGABIT_ETHERNET_TXRATE, FAST_GIGABIT_ETHERNET_TXRATE);
    }

    bitTime = 1/(double)txrate;

    // set slot time
    if (txrate==ETHERNET_TXRATE || txrate==FAST_ETHERNET_TXRATE)
        slotTime = SLOT_TIME;
    else
        slotTime = GIGABIT_SLOT_TIME;

    // only if Gigabit Ethernet
    frameBursting = (txrate==GIGABIT_ETHERNET_TXRATE || txrate==FAST_GIGABIT_ETHERNET_TXRATE);
    carrierExtension = (slotTime == GIGABIT_SLOT_TIME && !duplexMode);

    interFrameGap = INTERFRAME_GAP_BITS/(double)txrate;
    shortestFrameDuration = carrierExtension ? GIGABIT_MIN_FRAME_WITH_EXT : MIN_ETHERNET_FRAME;
}

void ModifiedEtherMACBase::printParameters()
{
    // Dump parameters
    EV << "MAC address: " << address << (promiscuous ? ", promiscuous mode" : "") << endl;
    EV << "txrate: " << txrate << ", " << (duplexMode ? "duplex" : "half-duplex") << endl;
#if 1
    EV << "bitTime: " << bitTime << endl;
    EV << "carrierExtension: " << carrierExtension << endl;
    EV << "frameBursting: " << frameBursting << endl;
    EV << "slotTime: " << slotTime << endl;
    EV << "interFrameGap: " << interFrameGap << endl;
    EV << endl;
#endif
}

void ModifiedEtherMACBase::processFrameFromUpperLayer(EtherFrame *frame)
{
    //EV << "Received frame from upper layer: " << frame << endl;

    if (frame->getDest().equals(address))
    {
        error("logic error: frame %s from higher layer has local MAC address as dest (%s)",
              frame->getFullName(), frame->getDest().str().c_str());
    }

    if (frame->getByteLength() > MAX_ETHERNET_FRAME)
        error("packet from higher layer (%d bytes) exceeds maximum Ethernet frame size (%d)", frame->getByteLength(), MAX_ETHERNET_FRAME);

    // must be EtherFrame (or EtherPauseFrame) from upper layer
        numFramesFromHL++;

        if (txQueueLimit && txQueue.length()>txQueueLimit)
        {
        	EthernetIIFrame *packet = check_and_cast<EthernetIIFrame *>(frame);
            vlanMessage *vlanTag = NULL;
            dataMessage *messageData = NULL;
            hsrMessage *hsrTag = NULL;
            MessagePacker::decapsulateMessage(&packet, &vlanTag, &hsrTag , &messageData);
            MessagePacker::deleteMessage(&packet, &vlanTag, &hsrTag , &messageData);

//                EV << "txQueue overflow" << endl;
//                error("txQueue length exceeds %d -- this is probably due to "
//                      "a bogus app model generating excessive traffic "
//                      "(or if this is normal, increase txQueueLimit!)",
//                      txQueueLimit);

            numFramesDroppedQueueFull++;
            numDroppedQueueFullVector.record(numFramesDroppedQueueFull);
        }
        else
        {
            // fill in src address if not set
            if (frame->getSrc().isUnspecified())
                frame->setSrc(address);

            // store frame and possibly begin transmitting
            //EV << "Packet " << frame << " arrived from higher layers, enqueueing\n";
            txQueue.insert(frame);
            if(txQueue.length() > 0)
                queueLengthVector.record(txQueue.length()-1);
            else
                queueLengthVector.record(0);
            if (transmitState == TX_IDLE_STATE)
                startFrameTransmission();
        }
}


void ModifiedEtherMACBase::processMsgFromNetwork(cPacket *frame)
{
    //EV << "Received frame from network: " << frame << endl;

    // frame must be EtherFrame or EtherJam
    if (dynamic_cast<EtherFrame*>(frame)==NULL && dynamic_cast<EtherJam*>(frame)==NULL)
        error("message with unexpected message class '%s' arrived from network (name='%s')",
                frame->getClassName(), frame->getFullName());

    // detect cable length violation in half-duplex mode
    if (!duplexMode && simTime()-frame->getSendingTime()>=shortestFrameDuration)
        error("very long frame propagation time detected, maybe cable exceeds maximum allowed length? "
              "(%lgs corresponds to an approx. %lgm cable)",
              SIMTIME_STR(simTime() - frame->getSendingTime()),
              SIMTIME_STR((simTime() - frame->getSendingTime())*MODIFIED_SPEED_OF_LIGHT));

    EtherFrame *Eframe = check_and_cast<EtherFrame *>(frame);
    //EV << "process msg from network " << frame << " simTime " << simTime() <<endl;
    if (hasSubscribers)
    {
        // fire notification
        notifDetails.setPacket(Eframe);
        //nb->fireChangeNotification(NF_PP_RX_END, &notifDetails);
    }

    if (checkDestinationAddress(Eframe))
        processReceivedDataFrame(Eframe);
}

// void ModifiedEtherMACBase::frameReceptionComplete(EtherFrame *frame)
// {

//         processReceivedDataFrame((EtherFrame *)frame);
// }

void ModifiedEtherMACBase::processReceivedDataFrame(EtherFrame *frame)
{
    // bit errors
    if (frame->hasBitError())
    {
        numDroppedBitError++;
        numDroppedBitErrorVector.record(numDroppedBitError);
    	EthernetIIFrame *packet = check_and_cast<EthernetIIFrame *>(frame);
        vlanMessage *vlanTag = NULL;
        dataMessage *messageData = NULL;
        hsrMessage *hsrTag = NULL;
        MessagePacker::decapsulateMessage(&packet, &vlanTag, &hsrTag , &messageData);
        MessagePacker::deleteMessage(&packet, &vlanTag, &hsrTag , &messageData);
        frame = NULL;
        return;
    }

    // strip preamble and SFD
    frame->addByteLength(-PREAMBLE_BYTES-SFD_BYTES);

    // statistics
    numFramesReceivedOK++;
    numBytesReceivedOK += frame->getByteLength();
    numFramesReceivedOKVector.record(numFramesReceivedOK);
    numBytesReceivedOKVector.record(numBytesReceivedOK);

    if (!checkDestinationAddress(frame))
        return;

    numFramesPassedToHL++;
    numFramesPassedToHLVector.record(numFramesPassedToHL);

    // pass up to upper layer
    send(frame, "upperLayerOut");
}

void ModifiedEtherMACBase::handleEndIFGPeriod()
{
    if (transmitState!=WAIT_IFG_STATE)
        error("Not in WAIT_IFG_STATE at the end of IFG period");

    if (txQueue.empty())
        error("End of IFG and no frame to transmit");

    // End of IFG period, okay to transmit, if Rx idle OR duplexMode
    cPacket *frame = (cPacket *)txQueue.front();
    //EV << "IFG elapsed, now begin transmission of frame " << frame << endl;

    // Perform carrier extension if in Gigabit Ethernet
    if (carrierExtension && frame->getByteLength() < GIGABIT_MIN_FRAME_WITH_EXT)
    {
        EV << "Performing carrier extension of small frame\n";
        frame->setByteLength(GIGABIT_MIN_FRAME_WITH_EXT);
    }

    // start frame burst, if enabled
    if (frameBursting)
    {
        EV << "Starting frame burst\n";
        framesSentInBurst = 0;
        bytesSentInBurst = 0;
    }

    startFrameTransmission();
}

void ModifiedEtherMACBase::handleEndTxPeriod()
{
    if (hasSubscribers)
    {
        // fire notification
        notifDetails.setPacket((cPacket *)txQueue.front());
//        nb->fireChangeNotification(NF_PP_TX_END, &notifDetails);
    }

    // we only get here if transmission has finished successfully, without collision
    if (transmitState!=TRANSMITTING_STATE || (!duplexMode && receiveState!=RX_IDLE_STATE))
        error("End of transmission, and incorrect state detected");

    if (txQueue.empty())
        error("Frame under transmission cannot be found");

    // get frame from buffer
    cPacket *frame = (cPacket *)txQueue.pop();
    if(txQueue.length() > 0)//1Frame in der Queue wird ja direkt gesendent, und wird darum ned gez�hlt
        queueLengthVector.record(txQueue.length()-1);
    else
        queueLengthVector.record(0);

    numFramesSent++;
    numBytesSent += frame->getByteLength();
    numFramesSentVector.record(numFramesSent);
    numBytesSentVector.record(numBytesSent);

    //EV << "Transmission of " << frame << " successfully completed\n";
	EthernetIIFrame *packet = check_and_cast<EthernetIIFrame *>(frame);
    vlanMessage *vlanTag = NULL;
    dataMessage *messageData = NULL;
    hsrMessage *hsrTag = NULL;
    MessagePacker::decapsulateMessage(&packet, &vlanTag, &hsrTag , &messageData);
    MessagePacker::deleteMessage(&packet, &vlanTag, &hsrTag , &messageData);
    frame = NULL;


    beginSendFrames();
}


void ModifiedEtherMACBase::processMessageWhenNotConnected(cMessage *msg)
{
    EV << "Interface is not connected -- dropping packet " << msg << endl;
    delete msg;
    numDroppedIfaceDown++;
}

void ModifiedEtherMACBase::processMessageWhenDisabled(cMessage *msg)
{
    EV << "MAC is disabled -- dropping message " << msg << endl;
    delete msg;
}

void ModifiedEtherMACBase::scheduleEndIFGPeriod()
{
    scheduleAt(simTime()+interFrameGap, endIFGMsg);
    transmitState = WAIT_IFG_STATE;
}

void ModifiedEtherMACBase::scheduleEndTxPeriod(cPacket *frame)
{
    //EV << "ModifiedEtherMACBase BitLength: " << frame->getBitLength() << endl;
    scheduleAt(simTime()+frame->getBitLength()*bitTime, endTxMsg);
    transmitState = TRANSMITTING_STATE;
}

void ModifiedEtherMACBase::beginSendFrames()
{
    if (!txQueue.empty())
    {
        // Other frames are queued, therefore wait IFG period and transmit next frame
        //EV << "Transmit next frame in output queue, after IFG period\n";
        scheduleEndIFGPeriod();
    }
    else
    {
        transmitState = TX_IDLE_STATE;
        // No more frames set transmitter to idle
        //EV << "No more frames to send, transmitter set to idle\n";
    }
}

void ModifiedEtherMACBase::fireChangeNotification(int type, cPacket *msg)
{
    if (nb) {
        notifDetails.setPacket(msg);
//        nb->fireChangeNotification(type, &notifDetails);
    }
}

void ModifiedEtherMACBase::finish()
{
    if (!disabled)
    {
        simtime_t t = simTime();
        recordScalar("simulated time", t);
        recordScalar("txrate (Mb)", txrate/1000000);
        recordScalar("full duplex", duplexMode);
        recordScalar("frames sent",    numFramesSent);
        recordScalar("frames rcvd",    numFramesReceivedOK);
        recordScalar("bytes sent",     numBytesSent);
        recordScalar("bytes rcvd",     numBytesReceivedOK);
        recordScalar("frames from higher layer", numFramesFromHL);
        recordScalar("frames from higher layer dropped (iface down)", numDroppedIfaceDown);
        recordScalar("frames dropped (bit error)",  numDroppedBitError);
        recordScalar("frames dropped (not for us)", numDroppedNotForUs);
        recordScalar("frames passed up to HL", numFramesPassedToHL);


        if (t>0)
        {
            recordScalar("frames/sec sent", numFramesSent/t);
            recordScalar("frames/sec rcvd", numFramesReceivedOK/t);
            recordScalar("bits/sec sent",   8*numBytesSent/t);
            recordScalar("bits/sec rcvd",   8*numBytesReceivedOK/t);
        }
    }
}

void ModifiedEtherMACBase::updateDisplayString()
{
    // icon coloring
    const char *color;
    if (receiveState==RX_COLLISION_STATE)
        color = "red";
    else if (transmitState==TRANSMITTING_STATE)
        color = "yellow";
    else if (receiveState==RECEIVING_STATE)
        color = "#4040ff";
    else
        color = "";
    getDisplayString().setTagArg("i",1,color);
    if (!strcmp(getParentModule()->getClassName(),"EthernetInterface"))
        getParentModule()->getDisplayString().setTagArg("i",1,color);

    // connection coloring
    updateConnectionColor(transmitState);

#if 0
    // this code works but didn't turn out to be very useful
    const char *txStateName;
    switch (transmitState) {
        case TX_IDLE_STATE:      txStateName="IDLE"; break;
        case WAIT_IFG_STATE:     txStateName="WAIT_IFG"; break;
        case TRANSMITTING_STATE: txStateName="TX"; break;
        case JAMMING_STATE:      txStateName="JAM"; break;
        case BACKOFF_STATE:      txStateName="BACKOFF"; break;
        case PAUSE_STATE:        txStateName="PAUSE"; break;
        default: error("wrong tx state");
    }
    const char *rxStateName;
    switch (receiveState) {
        case RX_IDLE_STATE:      rxStateName="IDLE"; break;
        case RECEIVING_STATE:    rxStateName="RX"; break;
        case RX_COLLISION_STATE: rxStateName="COLL"; break;
        default: error("wrong rx state");
    }

    char buf[80];
    sprintf(buf, "tx:%s rx: %s\n#boff:%d #cTx:%d",
                 txStateName, rxStateName, backoffs, numConcurrentTransmissions);
    getDisplayString().setTagArg("t",0,buf);
#endif
}

void ModifiedEtherMACBase::updateConnectionColor(int txState)
{
    const char *color;
    if (txState==TRANSMITTING_STATE)
        color = "yellow";
    else
        color = "";

    cGate *g = physOutGate;
    while (g && g->getType()==cGate::OUTPUT)
    {
        g->getDisplayString().setTagArg("o",0,color);
        g->getDisplayString().setTagArg("o",1, color[0] ? "3" : "1");
        g = g->getNextGate();
    }
}

// void ModifiedEtherMACBase::receiveChangeNotification(int category, const cPolymorphic *)
// {
//     if (category==NF_SUBSCRIBERLIST_CHANGED)
//         updateHasSubcribers();
// }

void ModifiedEtherMACBase::handleMessage(cMessage *msg)
{
    if (!connected)
        processMessageWhenNotConnected(msg);
    else if (disabled)
        processMessageWhenDisabled(msg);
    else if (msg->isSelfMessage())
    {
        //EV << "Self-message " << msg << " received\n";
        if (msg == endTxMsg)
            handleEndTxPeriod();
        else if (msg == endIFGMsg)
            handleEndIFGPeriod();
        else
            error("Unknown self message received!");
    }
    else
    {
        if (msg->getArrivalGate() == gate("upperLayerIn"))
            processFrameFromUpperLayer(check_and_cast<EtherFrame *>(msg));
        else if (msg->getArrivalGate() == gate("phys$i"))
            processMsgFromNetwork(check_and_cast<EtherFrame *>(msg));
        else
            error("Message received from unknown gate!");
    }

    if (ev.isGUI())  updateDisplayString();
}




void ModifiedEtherMACBase::startFrameTransmission()
{
    EtherFrame *origFrame = (EtherFrame *)txQueue.front();
    //EV << "Transmitting a copy of frame " << origFrame << endl;

    while(duplicateDetection->checkFrame(check_and_cast<EthernetIIFrame *>(origFrame->dup())) != 1)
    {
        txQueue.pop();
    	EthernetIIFrame *packet = check_and_cast<EthernetIIFrame *>(origFrame);
        vlanMessage *vlanTag = NULL;
        dataMessage *messageData = NULL;
        hsrMessage *hsrTag = NULL;
        MessagePacker::decapsulateMessage(&packet, &vlanTag, &hsrTag , &messageData);
        MessagePacker::deleteMessage(&packet, &vlanTag, &hsrTag , &messageData);
        origFrame = NULL;
        
        if(txQueue.empty()){
            //only duplicates in queue
            transmitState = TX_IDLE_STATE;
            return;
        }            
        origFrame = (EtherFrame *)txQueue.front();
    }
    
    EtherFrame *frame = (EtherFrame *) origFrame->dup();
    frame->addByteLength(PREAMBLE_BYTES+SFD_BYTES);

    if (hasSubscribers)
    {
        // fire notification
        notifDetails.setPacket(frame);
//        nb->fireChangeNotification(NF_PP_TX_BEGIN, &notifDetails);
    }

    // fill in src address if not set
    if (frame->getSrc().isUnspecified())
        frame->setSrc(address);

    // send
    //EV << "Starting transmission of " << frame << " simTime " << simTime() <<endl;
    
    scheduleEndTxPeriod(frame);

    // update burst variables
    if (frameBursting)
    {
        bytesSentInBurst = frame->getByteLength();
        framesSentInBurst++;
    }    
    
    send(frame, physOutGate);
}


// void ModifiedEtherMACBase::updateHasSubcribers()
// {
//     hasSubscribers = nb->hasSubscribers(NF_PP_TX_BEGIN) ||
//                      nb->hasSubscribers(NF_PP_TX_END) ||
//                      nb->hasSubscribers(NF_PP_RX_END);
// }
