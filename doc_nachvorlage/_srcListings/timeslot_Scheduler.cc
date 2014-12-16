...
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
        /*
	 * if ( time modulo (2 * phasesize) ) < phasesize
	 * the high-frame can be sent
	 */
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
...
