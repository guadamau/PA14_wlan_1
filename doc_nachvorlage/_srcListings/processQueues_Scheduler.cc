...
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
         * EXPRESS_RING, EXPRESS_INTERNAL, HIGH_RING, HIGH_INTERNAL, LOW_RING, LOW_INTERNAL
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
         * EXPRESS_RING, EXPRESS_INTERNAL, HIGH_RING, HIGH_INTERNAL, LOW_RING, LOW_INTERNAL
         *
         * [2n-1] odd
         * EXPRESS_INTERNAL, EXPRESS_RING, HIGH_INTERNAL, HIGH_RING, LOW_INTERNAL, LOW_RING
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
...
    }
}

...

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
...
