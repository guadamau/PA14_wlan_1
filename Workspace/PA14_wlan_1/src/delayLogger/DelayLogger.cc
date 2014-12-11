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
#include "math.h"
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

    delete this->preemptionTimeAdditionVecHigh;
    delete this->preemptionTimeAdditionVecLow;

    delete this->preemptionTimeAdditionStatsHigh;
    delete this->preemptionTimeAdditionStatsLow;
}

void DelayLogger::initialize( void )
{
    int histogramNumCells = par( "histogramNumCells" );
    int numFirstVals = par( "numFirstVals" );
    double rangeExtFactor = par( "rangeExtFactor" );

    /* create the instances */
    this->endToEndDelayVecExpress = new cOutVector();
    this->endToEndDelayVecHigh = new cOutVector();
    this->endToEndDelayVecLow = new cOutVector();

    this->endToEndDelayStatsExpress = new cDoubleHistogram( "Express Prio Delay", histogramNumCells );
    this->endToEndDelayStatsHigh = new cDoubleHistogram( "High Prio Delay", histogramNumCells );
    this->endToEndDelayStatsLow = new cDoubleHistogram( "Low Prio Delay", histogramNumCells );

    this->preemptionTimeAdditionVecHigh = new cOutVector();
    this->preemptionTimeAdditionVecLow = new cOutVector();

    this->preemptionTimeAdditionStatsHigh = new cDoubleHistogram( "High Priority Preemption Time Addition", histogramNumCells );
    this->preemptionTimeAdditionStatsLow = new cDoubleHistogram( "Low Priority Preemption Time Addition", histogramNumCells );


    /* Set initial stuff to the statistics tools in use ... */
    this->endToEndDelayVecExpress->setName( "End-to-End Delay EXP" );
    this->endToEndDelayVecHigh->setName( "End-to-End Delay HIGH" );
    this->endToEndDelayVecLow->setName( "End-to-End Delay LOW" );

    this->preemptionTimeAdditionVecHigh->setName( "Time Preemption of HIGH Frames" );
    this->preemptionTimeAdditionVecLow->setName( "Time Preemption of LOW Frames" );

    // 0.0 is lower limit
    this->endToEndDelayStatsExpress->setRangeAutoUpper( 0.0, numFirstVals, rangeExtFactor );
    this->endToEndDelayStatsHigh->setRangeAutoUpper( 0.0, numFirstVals, rangeExtFactor );
    this->endToEndDelayStatsLow->setRangeAutoUpper( 0.0, numFirstVals, rangeExtFactor );

    this->preemptionTimeAdditionStatsHigh->setRangeAutoUpper( 0.0, numFirstVals, rangeExtFactor );
    this->preemptionTimeAdditionStatsLow->setRangeAutoUpper( 0.0, numFirstVals, rangeExtFactor );
}

void DelayLogger::addDelay( cMessage* msg )
{
    EthernetIIFrame* frame = NULL;
    vlanMessage* vlanTag = NULL;
    hsrMessage* hsrTag = NULL;
    dataMessage* messageData = NULL;

    framePriority frameprio;

    take( msg );

    /* Lets get the priority and the creation time of the message. */
    frame = check_and_cast<EthernetIIFrame*>( msg );

    simtime_t frameCreationTime = frame->getCreationTime();
    EV << "[   ARRIVAL TIME:   ] (" << simTime() << ") "<< frame->getArrivalTime();

    MessagePacker::decapsulateMessage( &frame, &vlanTag, &hsrTag, &messageData );

    frameprio = static_cast<framePriority>( vlanTag->getUser_priority() );

    simtime_t msgDelay = simTime() - frameCreationTime + vlanTag->getPreemptionDelay();

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

            preemptionTimeAdditionVecHigh->record( vlanTag->getPreemptionDelay() );
            preemptionTimeAdditionStatsHigh->collect( vlanTag->getPreemptionDelay() );
            break;
        }

        case LOW:
        default:
        {
            endToEndDelayVecLow->record( msgDelay );
            endToEndDelayStatsLow->collect( msgDelay );

            preemptionTimeAdditionVecLow->record( vlanTag->getPreemptionDelay() );
            preemptionTimeAdditionStatsLow->collect( vlanTag->getPreemptionDelay() );
            break;
        }
    }

    MessagePacker::deleteMessage( &frame, &vlanTag, &hsrTag, &messageData );
}

void DelayLogger::finish( void )
{
    this->endToEndDelayStatsExpress->recordAs( "Express Prio Delay" );
    this->endToEndDelayStatsHigh->recordAs( "High Prio Delay" );
    this->endToEndDelayStatsLow->recordAs( "Low Prio Delay" );

    this->preemptionTimeAdditionStatsHigh->recordAs( "High Prio Frame Preemption Time" );
    this->preemptionTimeAdditionStatsLow->recordAs( "Low Prio Frame Preemption Time" );
}
