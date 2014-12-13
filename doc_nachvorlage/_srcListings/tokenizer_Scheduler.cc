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

...

    if( framePrio == LOW )
    {
        /* Time elapsed since last sent low priority frame. */
        simtime_t timeElapsed = simTime() - lowSendTime;
        double creditBytes = timeElapsed.dbl() * ( framebyteLimit / 8.0 );
        int64_t creditBytesFloored = ( int64_t )floor( creditBytes );

        /* in case of an overflow the result of ( framebytecontainer + creditBytes ) is negative */
        if( ( framebytecontainer + creditBytesFloored ) > framebyteLimit || ( framebytecontainer + creditBytesFloored ) < -1 )
        {
            framebytecontainer = framebyteLimit;
        }
        else
        {
            framebytecontainer = framebytecontainer + creditBytesFloored;
        }


        cGate* arrivalGate = msg->getArrivalGate();
        if( arrivalGate == parentSwitch->getGateCpuIn() || arrivalGate == parentSwitch->getGateCpuInExp() )
        {
            EthernetIIFrame* checkmsg = check_and_cast<EthernetIIFrame*>( msg->dup() );
            int64_t lengthFrame = checkmsg->getByteLength();
            delete checkmsg;

	    /* value of framebytelimit is -1 if the tokenizer is disabled -> return true */
            if( framebyteLimit == -1 )
            {
                retVal = 0x01;
            }
            else if( framebytecontainer - lengthFrame >= 0 )
            {
                /* in this case the low priority frame can be sent at this time. */
                retVal = 0x01;
                lowSendTime = simTime() - (creditBytes-creditBytesFloored)/framebyteLimit;
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

...

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

...

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
            else if ( framebyteLimit != -1 )
            {
                throw cRuntimeError( "[ Scheduler ]: Error in traffic limitation" );
                parentSwitch->endSimulation();
            }
        }
    }
}
