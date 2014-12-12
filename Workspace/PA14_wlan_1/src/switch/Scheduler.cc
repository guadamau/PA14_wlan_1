/*
 * Scheduler.cc
 *
 *  Created on: Oct 9, 2014
 *      Author: guadagnini
 */

#include <Ethernet.h>
#include <sstream>
#include <string>
#include <math.h>
#include <algorithm>
#include <float.h>
#include <csimulation.h>
#include "Scheduler.h"
#include "HsrSwitch.h"
#include "hsrSwitchSelfMessage_m.h"
#include "schedulerSelfMessage_m.h"
#include "MessagePacker.h"
#include "hsrMessage_m.h"
#include "EtherFrame_m.h"
#include "dataMessage_m.h"
#include "vlanMessage_m.h"


Scheduler::Scheduler()
{
    this->queues = new cArray();
    this->queueVectors = new cArray();
}

cArray* Scheduler::getQueues( void ) {
    return queues;
}

void Scheduler::setSchedmode(schedulerMode schedmode) {
    this->schedmode = schedmode;
}

schedulerMode Scheduler::getSchedmode( void ) {
    return schedmode;
}

SendingStatus* Scheduler::getSendingStatus( void )
{
    return sendingStatus;
}


Scheduler::~Scheduler()
{
    /* do some garbage collection. */
    int i;

    for( i = 0; i < queues->getCapacity(); i++ )
    {
        delete queues->get( i );
    }

    delete queues;
    delete queueVectors;

    delete sendingStatus;
}


void Scheduler::initScheduler( unsigned char schedID, HsrSwitch* parentSwitch, schedulerMode schedmode,
                               cGate* schedOutGate, cGate* schedOutGateExp,
                               NetworkInterfaceCard* schedNic, NetworkInterfaceCard* schedNicExp )
{
    this->schedID = schedID;

    this->transmitLock = 0;
    this->transmitLockExp = 0;

    this->finishTime = SIMTIME_ZERO;
    this->expSendTime = SIMTIME_ZERO;

    this->lowSendTime = SIMTIME_ZERO;

    this->parentSwitch = parentSwitch;

    this->schedmode = schedmode;

    this->preemptedFrames = 0;

    this->preemptedFramesVector = new cOutVector();

    std::stringstream stringStream;
    stringStream << schedID << ": " << parentSwitch->getFullName() << " Preempted Frames";
    this->preemptedFramesVector->setName( stringStream.str().c_str() );
    this->preemptedFramesVector->record( this->preemptedFrames );

    queues->addAt( EXPRESS_RING, new cQueue() );
    queues->addAt( EXPRESS_INTERNAL, new cQueue() );
    queues->addAt( HIGH_RING, new cQueue() );
    queues->addAt( HIGH_INTERNAL, new cQueue() );
    queues->addAt( LOW_RING, new cQueue() );
    queues->addAt( LOW_INTERNAL, new cQueue() );

    this->schedOutGate = schedOutGate;
    this->schedOutGateExp = schedOutGateExp;

    this->schedNic = schedNic;
    this->schedNicExp = schedNicExp;
    this->datarate = schedNic->getTransmissionChannel()->getNominalDatarate();

    this->curQueueState = RING;

    this->sendingStatus = new SendingStatus();
    this->msgExp = NULL;

    /* Members to record statistics. */
    const char* queueNamesStr[ QUEUES_COUNT ]={
            "QueueSize EXPRESS from Ring",
            "QueueSize EXPRESS (from Internal)",
            "QueueSize HIGH from Ring",
            "QueueSize HIGH (from Internal)",
            "QueueSize LOW from Ring",
            "QueueSize LOW (from Internal)"
    };

    for( int i = 0; i < QUEUES_COUNT; i++ )
    {
        queueSizes[ i ] = 0;

        cOutVector* queueVector = new cOutVector();

        std::stringstream ss;
        ss << schedID << ": " << queueNamesStr[ i ];
        queueVector->setName( ss.str().c_str() );

        queueVector->record( queueSizes[ i ] );
        queueVectors->addAt( i, queueVector );
    }

    /*
     * Define order for various mechanisms
     */
    ringFirstSortOrder[ 0 ] = EXPRESS_RING;
    ringFirstSortOrder[ 1 ] = EXPRESS_INTERNAL;
    ringFirstSortOrder[ 2 ] = HIGH_RING;
    ringFirstSortOrder[ 3 ] = HIGH_INTERNAL;
    ringFirstSortOrder[ 4 ] = LOW_RING;
    ringFirstSortOrder[ 5 ] = LOW_INTERNAL;

    internalFirstSortOrder[ 0 ] = EXPRESS_INTERNAL;
    internalFirstSortOrder[ 1 ] = EXPRESS_RING;
    internalFirstSortOrder[ 2 ] = HIGH_INTERNAL;
    internalFirstSortOrder[ 3 ] = HIGH_RING;
    internalFirstSortOrder[ 4 ] = LOW_INTERNAL;
    internalFirstSortOrder[ 5 ] = LOW_RING;

    this->timeslotPhaseSize = parentSwitch->par( "timeslotPhaseSize" );
    this->notifiedTimeslot = 0;
    this->framebyteLimit = parentSwitch->par("framebyteLimit");
    this->framebytecontainer = framebyteLimit;
    this->notifiedToken = 0;
}


void Scheduler::enqueueMessage( cMessage *msg, queueName queue )
{
    if(schedmode == FCFS)
    {
        if(queue == EXPRESS_RING) { queue = EXPRESS_INTERNAL; }
        else if(queue == HIGH_RING) { queue = HIGH_INTERNAL; }
        else if(queue == LOW_RING) { queue = LOW_INTERNAL; }
    }

    ( check_and_cast<cQueue*>( queues->get( queue ) ) )->insert( msg );

    /* LOGGING */
    cOutVector* currentQueueSizeVector = check_and_cast<cOutVector*>( queueVectors->get( queue ) );
    currentQueueSizeVector->record( ++queueSizes[queue] );
}

void Scheduler::processQueues( void )
{
    queueName* currentSortOrder;


    switch( schedmode )
    {
        /*
         * FCFS: we have no distinction between Ring- and Internal frames.
         * See the function above "enqueueMessage". Frames ordered to the ring queues
         * are redirected to the internal queues.
         * Due to the above description, the behaviour FCFS and RING_FIRST have not to
         * be distinguished here.
         *
         * The Ring queues of each level are processed first,
         * which is part of the RING_FIRST policy.
         *
         * In case of FCFS-scheduling-mode the ring queues are just always empty,
         * see enqueueMessage-function.
         *
         * The processing order of the queues is as follows:
         *
         * EXPRESS_RING,
         * EXPRESS_INTERNAL,
         * HIGH_RING,
         * HIGH_INTERNAL,
         * LOW_RING,
         * LOW_INTERNAL
         * */
        case FCFS:
        case RING_FIRST:
        {
            currentSortOrder = ringFirstSortOrder;
            loopQueues( currentSortOrder );
            break;
        }

        /*
         * The Internal queues of each level are processed first,
         * which is part of the INTERNAL_FIRST policy.
         */
        case INTERNAL_FIRST:
        {
            currentSortOrder = internalFirstSortOrder;
            loopQueues( currentSortOrder );
            break;
        }

        /*
         * ZIPPER-Policy
         * Alternates the queues between ring and internal.
         *
         * Processing order of the queues:
         *
         * [2n] even
         * EXPRESS_RING,
         * EXPRESS_INTERNAL,
         * HIGH_RING,
         * HIGH_INTERNAL,
         * LOW_RING,
         * LOW_INTERNAL
         *
         * [2n-1] odd
         * EXPRESS_INTERNAL,
         * EXPRESS_RING,
         * HIGH_INTERNAL,
         * HIGH_RING,
         * LOW_INTERNAL,
         * LOW_RING
         * */
        case ZIPPER:
        {
            switch( curQueueState )
            {
                case RING:
                {
                    currentSortOrder = ringFirstSortOrder;
                    loopQueues( currentSortOrder );
                    curQueueState = INTERNAL;
                    break;
                }
                case INTERNAL:
                {
                    currentSortOrder = internalFirstSortOrder;
                    loopQueues( currentSortOrder );
                    curQueueState = RING;
                    break;
                }
                default:
                {
                    break;
                }
            }
            /* break to case ZIPPER */
            break;
        }

        /* TOKENS-Policy
         *
         * */
        case TOKENS:
        {
            /* break belongs to case TOKENS */
            break;
        }

        /* default to not fall through case statement ... */
        default:
        {
            break;
        }
    }
}

void Scheduler::sendMessage( cMessage* msg, cGate* outGate )
{
    parentSwitch->send( msg, outGate );
    return;
}

void Scheduler::loopQueues( queueName* currentSortOrder )
{
    cQueue* currentQueue = NULL;
    queueName currentQueueName;

    for( unsigned char i = 0; i < QUEUES_COUNT; i++ )
    {
        currentQueueName = currentSortOrder[ i ];
        currentQueue = check_and_cast<cQueue*>( queues->get( currentQueueName ) );

        if( currentQueue != NULL )
        {
            processOneQueue( currentQueue, currentQueueName );
        }
        else
        {
            throw cRuntimeError( " [ PANIC ] : current queue to process is NULL! exiting ... \n" );
        }
    }
}

unsigned char Scheduler::timeslotIsValid( queueName currentQueueName )
{
    simtime_t now = simTime();
    simtime_t phase = ( timeslotPhaseSize * 8.0 ) / datarate;
    /*
     * The interval contains two phases, one for high, one for low
     * An express-frame can always be sent
     */
    simtime_t interval = phase * 2;

    /*
     * Return true if the timeslot is disabled (size = 0)
     * An express-frame can always be sent
     *
     * A low frame can also always be sent, because if there are no high frames and only low frames,
     * they can be sent in the phase for the high frames and high frames always come before low frames
     */
    if( timeslotPhaseSize == 0 || currentQueueName == EXPRESS_RING || currentQueueName == EXPRESS_INTERNAL ||
            currentQueueName == LOW_RING || currentQueueName == LOW_INTERNAL)
    {
        /*
         * If size is 0 the timeslot-mechanism is disabled
         */
        return 0x01;
    }
    else if( currentQueueName == HIGH_RING || currentQueueName == HIGH_INTERNAL )
    {
        if( fmod( now.dbl(), interval.dbl() ) < phase.dbl() )
        {
            return 0x01;
        }
        else
        {
            return 0x00;
        }
    }
    else
    {
        return 0x00;
    }
}

unsigned char Scheduler::containerHasEnoughBytes( cMessage* msg )
{
    unsigned char retVal = 0x00;
    /*
     * The tokenizer has only impact on low priority frames.
     * This method always returns true when the priority is not low.
     * */
    EthernetIIFrame* ethTag = NULL;
    vlanMessage* vlanTag = NULL;
    hsrMessage* hsrTag = NULL;
    dataMessage* ethPayload = NULL;

    ethTag = check_and_cast<EthernetIIFrame*>( msg->dup() );

    MessagePacker::decapsulateMessage( &ethTag, &vlanTag, &hsrTag, &ethPayload );

    framePriority framePrio = static_cast<framePriority>( vlanTag->getUser_priority() );

    if( framePrio != LOW && framePrio != HIGH && framePrio != EXPRESS )
    {
        throw cRuntimeError( "[ Scheduler ]: Can't handle this priority number. Exiting ..." );
        parentSwitch->endSimulation();
    }

    if( framePrio == LOW )
    {
        // parentSwitch->frameByteContainerCheck();

        /* Time elapsed since last sent low priority frame. */
        simtime_t timeElapsed = simTime() - lowSendTime;

        int64_t creditBytes = ( int64_t )floor( timeElapsed.dbl() * ( datarate / 8.0 ) );

        /* in case of an overflow the result of ( framebytecontainer + creditBytes ) is negative */
        if( ( framebytecontainer + creditBytes ) > framebyteLimit || ( framebytecontainer + creditBytes ) < -1 )
        {
            framebytecontainer = framebyteLimit;
        }
        else
        {
            framebytecontainer = framebytecontainer + creditBytes;
        }


        EV << endl << "FRAMEBYTE CONTAINER: " << framebytecontainer << endl;
        cGate* arrivalGate = msg->getArrivalGate();
        if( arrivalGate == parentSwitch->getGateCpuIn() || arrivalGate == parentSwitch->getGateCpuInExp() )
        {
            EthernetIIFrame* checkmsg = check_and_cast<EthernetIIFrame*>( msg->dup() );
            int64_t lengthFrame = checkmsg->getByteLength();
            delete checkmsg;

            if( framebyteLimit == -1 )
            {
                retVal = 0x01;
            }
            else if( framebytecontainer - lengthFrame >= 0 )
            {
                retVal = 0x01;
                /* in this case the low priority frame can be sent at this time. */
                lowSendTime = simTime();
            }
        }
        else
        {
            retVal = 0x01;
        }
    }
    else
    {
        retVal = 0x01;
    }

    MessagePacker::deleteMessage( &ethTag, &vlanTag, &hsrTag, &ethPayload );

    return retVal;
}

void Scheduler::subtractFromByteContainer( cMessage* msg )
{
    /*
     * The tokenizer has only impact on low priority frames.
     * This method does nothing when the priority is not low.
     * */
    EthernetIIFrame* ethTag = NULL;
    vlanMessage* vlanTag = NULL;
    hsrMessage* hsrTag = NULL;
    dataMessage* ethPayload = NULL;

    ethTag = check_and_cast<EthernetIIFrame*>( msg->dup() );

    MessagePacker::decapsulateMessage( &ethTag, &vlanTag, &hsrTag, &ethPayload );

    framePriority framePrio = static_cast<framePriority>( vlanTag->getUser_priority() );

    if( framePrio != LOW && framePrio != HIGH && framePrio != EXPRESS )
    {
        throw cRuntimeError( "[ Scheduler ]: Can't handle this priority number. Exiting ..." );
        parentSwitch->endSimulation();
    }

    MessagePacker::deleteMessage( &ethTag, &vlanTag, &hsrTag, &ethPayload );

    if( framePrio == LOW )
    {
        /*
         * Only subtract newly generated frames (from the cpu)
         */
        cGate* arrivalGate = msg->getArrivalGate();
        if( arrivalGate == parentSwitch->getGateCpuIn() || arrivalGate == parentSwitch->getGateCpuInExp() )
        {
            EthernetIIFrame* checkmsg = check_and_cast<EthernetIIFrame*>( msg->dup() );
            int64_t lengthFrame = checkmsg->getByteLength();
            delete checkmsg;

            if( framebytecontainer - lengthFrame >= 0 )
            {
                framebytecontainer = framebytecontainer - lengthFrame;
            }
        }
    }
}

void Scheduler::refreshContainer( void )
{
    framebytecontainer = framebyteLimit;
}

void Scheduler::unlock( void )
{
    transmitLock = 0;
}

void Scheduler::unlockExp( void )
{
    transmitLockExp = 0;
    msgExp = NULL;
}

void Scheduler::resetNotifiedToken( void )
{
    notifiedToken = 0;
}

void Scheduler::resetNotifiedTimeslot( void )
{
    notifiedTimeslot = 0;
}

void Scheduler::processOneQueue( cQueue* currentQueue, queueName currentQueueName )
{
    if( !currentQueue->isEmpty() )
    {
        /* LOGGING */
        cOutVector* currentQueueSizeVector = check_and_cast<cOutVector*>( queueVectors->get( currentQueueName ) );

        EthernetIIFrame* msgeth = check_and_cast<EthernetIIFrame*>( currentQueue->front()->dup() );
        unsigned char enoughBytesAvailableToSend = containerHasEnoughBytes( msgeth );

        /* check if the transmit state of the nic is idle. */
        if( ( transmitLock == 0 && transmitLockExp == 0 &&
              timeslotIsValid( currentQueueName ) && enoughBytesAvailableToSend )
            || schedID == 'C' )
        {
            cMessage* msg = check_and_cast<cMessage*>( currentQueue->pop() );

            if( !(currentQueueName == EXPRESS_RING) && !(currentQueueName == EXPRESS_INTERNAL) )
            {
                transmitLock = 1;
                /*
                 * Only attach Frames that are not from the "Express-Ring"- or "Express-Internal"-queue
                 */
                sendingStatus->attachFrame( msg );
                sendMessage( msg, schedOutGate );
                subtractFromByteContainer( msgeth );
                delete msgeth;
                currentQueueSizeVector->record( --queueSizes[ currentQueueName ] );
            }
            /*
             * Express-Frames
             */
            else
            {
                transmitLockExp = 1;
                sendMessage( msg, schedOutGateExp );
                subtractFromByteContainer( msgeth );
                delete msgeth;
                currentQueueSizeVector->record( --queueSizes[ currentQueueName ] );
            }
        }
        else
        {
            if( ( currentQueueName == EXPRESS_RING || currentQueueName == EXPRESS_INTERNAL ) &&
                    transmitLock == 1 &&
                    transmitLockExp == 0 &&
                    enoughBytesAvailableToSend )
            {
                /*
                 * EXPRESS HANDLING (PREEMPTION)
                 */
                expSendTime = getExpressSendTime();

                EV << endl << "[ ExpSendTime ]:" << expSendTime << endl;

                /* return SIMTIME_ZERO means current frame is not fragmentable */
                if( expSendTime != SIMTIME_ZERO )
                {
                    transmitLockExp = 1;
                    sendExpressFrame( currentQueue, currentQueueName, expSendTime );
                }
            }
            else
            {
                HsrSwitchSelfMessage* looklatermsg = new HsrSwitchSelfMessage();
                looklatermsg->setSchedulerName( schedID );
                simtime_t now = simTime();
                simtime_t nextlookup = SIMTIME_ZERO;
                int64_t curFrameLength = msgeth->getByteLength();

                if ( !enoughBytesAvailableToSend && notifiedToken == 0 )
                {
                    // parentSwitch->frameByteContainerCheck();
                    // nextlookup = floor(simTime().dbl()) + 1.0;
                    nextlookup =  simTime() + ( ( curFrameLength - framebytecontainer ) / ( datarate / 8.0 ) );

                    looklatermsg->setType('o');
                    notifiedToken = 1;
                    parentSwitch->scheduleAt( nextlookup, looklatermsg );
                }
                else if( !timeslotIsValid( currentQueueName ) && notifiedTimeslot == 0 )
                {
                    nextlookup = now.dbl() +
                            ((( timeslotPhaseSize * 8.0 ) / datarate)
                                    - fmod((now.dbl()), ((( timeslotPhaseSize * 8.0 ) / datarate))));
                    while ( now.dbl() == nextlookup.dbl() )
                    {
                        nextlookup += (( timeslotPhaseSize * 8.0 ) / datarate);
                    }
                    looklatermsg->setType('i');
                    notifiedTimeslot = 1;
                    parentSwitch->scheduleAt( nextlookup, looklatermsg );
                }

                if( nextlookup == SIMTIME_ZERO )
                {
                    delete looklatermsg;
                }
            }
        }
    }
}

void Scheduler::sendExpressFrame( cQueue* currentQueue, queueName currentQueueName, simtime_t expSendTime )
{
    cMessage* msgExp = check_and_cast<cMessage*>( currentQueue->pop() );
    EthernetIIFrame* msgethExp = check_and_cast<EthernetIIFrame*>( msgExp->dup() );
    parentSwitch->sendDelayed(msgExp, (expSendTime-simTime()), schedOutGateExp);

    /* LOGGING */
    cOutVector* currentQueueSizeVector = check_and_cast<cOutVector*>( queueVectors->get( currentQueueName ) );
    currentQueueSizeVector->record( --queueSizes[ currentQueueName ] );

    EthernetIIFrame* fcsMsg = new EthernetIIFrame();
    fcsMsg->setBitLength( 4 * 8 );
    EthernetIIFrame* ifgMsg = new EthernetIIFrame();
    ifgMsg->setBitLength( INTERFRAME_GAP_BITS );
    EthernetIIFrame* preambleMsg = new EthernetIIFrame();
    preambleMsg->setBitLength( 8 * 8 );

    cMessage* currMsg = sendingStatus->getMessage();

    simtime_t timeMfcsFirstFragment = schedNicExp->getTransmissionChannel()->calculateDuration( fcsMsg );
    simtime_t timeIfg = schedNicExp->getTransmissionChannel()->calculateDuration( ifgMsg );
    simtime_t timePreamble = schedNicExp->getTransmissionChannel()->calculateDuration( preambleMsg );
    simtime_t timeMsgExp = schedNicExp->getTransmissionChannel()->calculateDuration( msgExp );
    simtime_t timeMsgDelayWhenFragmented = timeMfcsFirstFragment + timeIfg + timePreamble + timeMsgExp + timeIfg + timePreamble;

    addPreemptionDelay( currMsg, timeMsgDelayWhenFragmented );

    /* Statistics ... */
    subtractFromByteContainer( msgethExp );
    delete msgethExp;
    this->preemptedFramesVector->record( ++this->preemptedFrames );

    msgExp = NULL;
}

simtime_t Scheduler::getExpressSendTime( void )
{
    /* Algorithm from documentation PA14_wlan_1 */
    int64_t allBytesOfSendingFrame = sendingStatus->getMessageSize();

    simtime_t simTimeNow = simTime();
    simtime_t currentPreemptionDelay = sendingStatus->getPreemptionDelay();

    simtime_t sendTimeOfFrame = sendingStatus->getSendingTime();
    simtime_t calcExpSendTime;


    int64_t bytesAlreadySent = ( int64_t )floor(
                ( ( simTimeNow.dbl() - sendTimeOfFrame.dbl() ) * datarate ) / 8.0
                + ( (currentPreemptionDelay.dbl() * datarate) / 8.0 )
            );
    int64_t bytesAlreadySentData = bytesAlreadySent - 12 - (INTERFRAME_GAP_BITS / 8);
    if( bytesAlreadySentData < 0 )
    {
        bytesAlreadySentData = 0;
    }


    int64_t bytesNotYetSent = allBytesOfSendingFrame - bytesAlreadySent + 12;


    if( bytesAlreadySent >= ( 72 + ( INTERFRAME_GAP_BITS / 8 ) ) &&
        bytesAlreadySentData % 8 == 0 &&
        bytesNotYetSent >= 64 )
    {
        /* can send express frame now. */
        calcExpSendTime = simTimeNow;
    }
    else if( bytesAlreadySentData % 8 != 0 &&
             bytesNotYetSent >= 64 )
    {
        if( bytesAlreadySent >= ( 72 + ( INTERFRAME_GAP_BITS / 8 ) ) )
        {
            calcExpSendTime = simTimeNow + ( ( ( 8 - ( bytesAlreadySentData % 8 ) + ( INTERFRAME_GAP_BITS / 8 ) ) * 8 ) / datarate );
        }
        else
        {
            calcExpSendTime = simTimeNow + ( ( ( 72 + ( INTERFRAME_GAP_BITS / 8 ) - bytesAlreadySent + ( 8 - ( bytesAlreadySentData % 8 ) ) ) * 8 ) / datarate );
        }
    }
    else if( bytesAlreadySentData % 8 == 0 &&
             bytesNotYetSent >= 64 )
    {
        if( bytesAlreadySent >= ( 72 + ( INTERFRAME_GAP_BITS / 8 ) ) )
        {
            calcExpSendTime = simTimeNow + ( ( ( ( INTERFRAME_GAP_BITS / 8 ) ) * 8 ) / datarate );
        }
        else
        {
            calcExpSendTime = simTimeNow + ( ( ( 72 + ( INTERFRAME_GAP_BITS / 8 ) - bytesAlreadySent ) * 8 ) / datarate );
        }
    }
    else
    {
        calcExpSendTime = SIMTIME_ZERO;
    }

    return calcExpSendTime;
}

void Scheduler::addPreemptionDelay( cMessage* msg, simtime_t delayCorrection )
{
    cMessage* message = msg->dup();

    EthernetIIFrame* ethTag = NULL;
    vlanMessage* vlanTag = NULL;
    hsrMessage* hsrTag = NULL;
    dataMessage* messageData = NULL;

    ethTag = check_and_cast<EthernetIIFrame*>( message );

    MessagePacker::decapsulateMessage( &ethTag, &vlanTag, &hsrTag, &messageData );

    vlanTag->setPreemptionDelay( vlanTag->getPreemptionDelay() + delayCorrection );
    sendingStatus->setMessageSize( sendingStatus->getMessageSize() + ( delayCorrection.dbl()  * datarate ) / 8.0 );

    msg = MessagePacker::generateEthMessage( ethTag, vlanTag, hsrTag, messageData );
}

