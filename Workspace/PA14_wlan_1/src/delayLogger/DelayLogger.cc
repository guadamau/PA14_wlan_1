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

#include "hsrDefines.h"
#include <delayLogger/DelayLogger.h>

#include "MessagePacker.h"
#include "hsrMessage_m.h"
#include "EtherFrame_m.h"
#include "dataMessage_m.h"
#include "vlanMessage_m.h"


Define_Module( DelayLogger );

DelayLogger::DelayLogger() {}

DelayLogger::~DelayLogger()
{
    delete this->endToEndDelayVecExpress;
    delete this->endToEndDelayVecHigh;
    delete this->endToEndDelayVecLow;

    delete this->endToEndDelayStatsExpress;
    delete this->endToEndDelayStatsHigh;
    delete this->endToEndDelayStatsLow;
}

void DelayLogger::initialize()
{
    this->endToEndDelayVecExpress = new cOutVector();
    this->endToEndDelayVecHigh = new cOutVector();
    this->endToEndDelayVecLow = new cOutVector();

    this->endToEndDelayStatsExpress = new cDoubleHistogram();
    this->endToEndDelayStatsHigh = new cDoubleHistogram();
    this->endToEndDelayStatsLow = new cDoubleHistogram();

    endToEndDelayVecExpress->setName( "End-to-End Delay EXP" );
    endToEndDelayVecHigh->setName( "End-to-End Delay HIGH" );
    endToEndDelayVecLow->setName( "End-to-End Delay LOW" );
}

void DelayLogger::addDelay( cMessage* msg )
{
    EthernetIIFrame* frame = NULL;
    vlanMessage* vlanTag = NULL;
    hsrMessage* hsrTag = NULL;
    dataMessage* messageData = NULL;

    framePriority frameprio;

    /* Lets get the priority and the creation time of the message. */
    frame = check_and_cast<EthernetIIFrame*>( msg );

    simtime_t frameCreationTime = frame->getCreationTime();

    simtime_t msgDelay = simTime() - frameCreationTime;

    MessagePacker::decapsulateMessage( &frame, &vlanTag, &hsrTag, &messageData );

    frameprio = static_cast<framePriority>( vlanTag->getUser_priority() );


    if( frameprio != LOW && frameprio != HIGH && frameprio != EXPRESS )
    {
        throw cRuntimeError( "[ DelayLogger ]: Can't handle this priority number. Exiting ..." );
        endSimulation();
    }

    switch( frameprio )
    {
        case EXPRESS:
        {
            endToEndDelayVecExpress->record( msgDelay );
            endToEndDelayStatsExpress->collect( msgDelay );
            break;
        }

        case HIGH:
        {
            endToEndDelayVecHigh->record( msgDelay );
            endToEndDelayStatsHigh->collect( msgDelay );
            break;
        }

        case LOW:
        default:
        {
            endToEndDelayVecLow->record( msgDelay );
            endToEndDelayStatsLow->collect( msgDelay );
            break;
        }
    }

    delete msg;
}

void DelayLogger::finish( void )
{
    this->endToEndDelayStatsExpress->recordAs( "Express Prio Delay" );
    this->endToEndDelayStatsHigh->recordAs( "High Prio Delay" );
    this->endToEndDelayStatsLow->recordAs( "Low Prio Delay" );
}
