...
void Scheduler::processOneQueue( cQueue* currentQueue, queueName currentQueueName )
{
    if( !currentQueue->isEmpty() )
    {
	...
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
		...
            }
            /*
             * Express-Frames
             */
            else
            {
                transmitLockExp = 1;
                sendMessage( msg, schedOutGateExp );
		...
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

                /* return SIMTIME_ZERO means current frame is not fragmentable */
                if( expSendTime != SIMTIME_ZERO )
                {
                    transmitLockExp = 1;
                    sendExpressFrame( currentQueue, currentQueueName, expSendTime );
                }
            }
            else
            {
		    ...
            }
        }
    }
}

simtime_t Scheduler::getExpressSendTime( void )
{
    /* Algorithm from documentation PA14_wlan_1 */
    int64_t allBytesOfSendingFrame = sendingStatus->getMessageSize();
    simtime_t calcExpSendTime;

    if( allBytesOfSendingFrame >= 128 )
    {

        simtime_t simTimeNow = simTime();
        simtime_t currentPreemptionDelay = sendingStatus->getPreemptionDelay();

        simtime_t sendTimeOfFrame = sendingStatus->getSendingTime();

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


        if( ( bytesAlreadySent >= ( 72 + ( INTERFRAME_GAP_BITS / 8 ) ) &&
            bytesAlreadySentData % 8 == 0 &&
            bytesNotYetSent >= 64 ) || nextUpAnotherExp == 1 )
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
    }
    else
    {
        calcExpSendTime = SIMTIME_ZERO;
    }

    return calcExpSendTime;
}

void Scheduler::sendExpressFrame( cQueue* currentQueue, queueName currentQueueName, simtime_t expSendTime )
{
    cMessage* msgExp = check_and_cast<cMessage*>( currentQueue->pop() );
    EthernetIIFrame* msgethExp = check_and_cast<EthernetIIFrame*>( msgExp->dup() );
    parentSwitch->sendDelayed(msgExp, (expSendTime-simTime()), schedOutGateExp);

    ...

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

    ...
}
...
