...
void EndNodeSwitch::handleMessage( cMessage* msg ) {
...
 /* Schedulers */
    Scheduler* schedGateAOut = HsrSwitch::getSchedGateAOut();
    Scheduler* schedGateBOut = HsrSwitch::getSchedGateBOut();
    Scheduler* schedGateCpuOut = HsrSwitch::getSchedGateCpuOut();
...
    /* Gates */
    cGate* gateAIn = HsrSwitch::getGateAIn();
    cGate* gateBIn = HsrSwitch::getGateBIn();
    cGate* gateCpuIn = HsrSwitch::getGateCpuIn();

    cGate* gateAInExp = HsrSwitch::getGateAInExp();
    cGate* gateBInExp = HsrSwitch::getGateBInExp();
    cGate* gateCpuInExp = HsrSwitch::getGateCpuInExp();
...
    /* Make a clone of the frame and take ownership.
     * Then we have to downcast the Message as an ethernet frame. */
    cMessage* switchesMsg = msg;
    this->take( switchesMsg );
...
    /* Arrival Gate */
    cGate* arrivalGate = switchesMsg->getArrivalGate();

    /* Switch mac address. */
    MACAddress switchMacAddress = *( HsrSwitch::getMacAddress() );

    EthernetIIFrame* ethernetFrame = check_and_cast<EthernetIIFrame*>( switchesMsg );

    /* Source and destination mac addresses */
    MACAddress frameDestination = ethernetFrame->getDest();
    MACAddress frameSource = ethernetFrame->getSrc();

    EthernetIIFrame* ethTag = NULL;
    vlanMessage* vlanTag = NULL;
    hsrMessage* hsrTag = NULL;
    dataMessage* messageData = NULL;

    EthernetIIFrame* frameToDeliver = NULL;
    EthernetIIFrame* frameToDeliverClone = NULL;

    framePriority frameprio = LOW;

    MessagePacker::decapsulateMessage( &ethernetFrame, &vlanTag, &hsrTag, &messageData );

    /* After decapsulating the whole ethernet frame becomes a ethernet tag
     * (only the header of an ethernet frame)
     * ethTag variable just there for a better understanding. */
    ethTag = ethernetFrame;

    /* determine package prio and set enum */
    if( vlanTag != NULL )
    {
        if( vlanTag->getUser_priority() == EXPRESS )
        {
            frameprio = EXPRESS;
        }
        else if( vlanTag->getUser_priority() == HIGH )
        {
            frameprio = HIGH;
        }
    }
...
        if( arrivalGate == gateCpuIn || arrivalGate == gateCpuInExp )
        {
            frameToDeliver = MessagePacker::generateEthMessage( ethTag, vlanTag, hsrTag, messageData );
            frameToDeliverClone = frameToDeliver->dup();

            switch( frameprio )
            {
                case EXPRESS:
                {
                    schedGateAOut->enqueueMessage( frameToDeliver, EXPRESS_INTERNAL );
                    schedGateBOut->enqueueMessage( frameToDeliverClone, EXPRESS_INTERNAL );
                    break;
                }
                case HIGH:
                {
                    schedGateAOut->enqueueMessage( frameToDeliver, HIGH_INTERNAL );
                    schedGateBOut->enqueueMessage( frameToDeliverClone, HIGH_INTERNAL );
                    break;
                }
                default:
                {
                    schedGateAOut->enqueueMessage( frameToDeliver, LOW_INTERNAL );
                    schedGateBOut->enqueueMessage( frameToDeliverClone, LOW_INTERNAL );
                    break;
                }
            }
            scheduleProcessQueues('A');
            scheduleProcessQueues('B');
        }  
...
}
...
