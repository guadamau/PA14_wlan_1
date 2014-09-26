
packet hsrMessage 
{
    unsigned int path; // 4Bit
    unsigned int sequencenumber; //16Bit
    unsigned int lsdu_size; //12Bit
    
    int nextEtherType;
    int ownEtherType = 0x88FB; //16bit
    
    //4+16+12+16 = 48 / 8 = 6 Byte extra HSR info
}