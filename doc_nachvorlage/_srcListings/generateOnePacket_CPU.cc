...
EthernetIIFrame* CPU::generateOnePacket(SendData sendData)
{
    const char* framePrioArr[] = {"EXPRESS", "HIGH", "LOW"};

    EthernetIIFrame *result_ethTag = NULL;

    const char* ethFrameName = "Uni";

    if(sendData.destination.isBroadcast()) {
        ethFrameName = "Broad";
    }
    else if(sendData.destination.isMulticast()) {
        ethFrameName = "Multi";
    }

    /*
     * Display Seq# in GUI
     */
    std::stringstream ss;
    ss << ethFrameName << " From: " << getParentModule()->getFullName() << " (" << framePrioArr[sendData.frameprio] <<") Seq#" << sequenceNumber;
    ethFrameName = ss.str().c_str();

    result_ethTag = MessagePacker::createETHTag( ethFrameName, sendData.destination, macAddress );

    hsrMessage *result_hsrTag = NULL;
    result_hsrTag = MessagePacker::createHSRTag( "HSR", 1, sequenceNumber );

    vlanMessage *result_vlanTag = NULL;
    result_vlanTag = MessagePacker::createVLANTag( "vlan", sendData.frameprio, 0, 0, 0 );

    dataMessage* result_dataMessage = NULL;
    result_dataMessage = MessagePacker::createDataMessage( "stdData", sendData.paketgroesse, messageCount++ );

    EthernetIIFrame* result_ethFrame = NULL;
    result_ethFrame = MessagePacker::generateEthMessage( result_ethTag, result_vlanTag, result_hsrTag, result_dataMessage );


    MessagePacker::deleteMessage(&result_ethTag, &result_vlanTag, &result_hsrTag, &result_dataMessage);

    EV << "[ OK ] " << sendData.frameprio << " Message created at: " << result_ethFrame->getCreationTime() << " created." << "  |  Seq#: " << sequenceNumber << endl;
    sequenceNumber++;

    return result_ethFrame;
}
...
