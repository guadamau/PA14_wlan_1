//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include <NetworkInterfaceCard.h>
#include "schedulerSelfMessage_m.h"

Define_Module( NetworkInterfaceCard );

NetworkInterfaceCard::NetworkInterfaceCard() {}

NetworkInterfaceCard::~NetworkInterfaceCard() {}

void NetworkInterfaceCard::initialize(int stage)
{
    EtherMACFullDuplex::initialize( stage );
    upperLayerOut = gate("upperLayerOut");
//    this->transmitLock = 0;
}

cGate* NetworkInterfaceCard::getPhysOutGate( void )
{
    return this->physOutGate;
}

cChannel* NetworkInterfaceCard::getTransmissionChannel( void )
{
    return this->transmissionChannel;
}

unsigned char NetworkInterfaceCard::getDeviceTransmitState( void )
{
    return this->transmitState;
}

void NetworkInterfaceCard::handleMessage( cMessage *msg )
{
    EtherMACFullDuplex::handleMessage( msg );

    /*
     * Transmitting of frame complete.
     * Notifiy the Scheduler, that the NIC is in Idle-State again
     * */
    if( msg == endIFGMsg && transmitState == TX_IDLE_STATE )
    {
//        transmitLock = 0;
        EV << "[ NIC " << simTime() << " ] ENDIFG reached! Unlock NIC!" << endl << endl;
        send( new SchedulerSelfMessage(), upperLayerOut );
    }
    else if( msg->getArrivalGate() == upperLayerInGate )
    {
//        transmitLock = 1;
    }
}

unsigned char NetworkInterfaceCard::isLocked( void )
{
    return this->transmitLock;
}

void NetworkInterfaceCard::lock( void )
{
    this->transmitLock = 1;
}

void NetworkInterfaceCard::unlock( void )
{
    this->transmitLock = 0;
}

cMessage* NetworkInterfaceCard::getEndIFGMsg( void )
{
    return this->endIFGMsg;
}
