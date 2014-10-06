//
// TODO generated message class
//

class EthernetIIFrame;   // let opp_msgc know "base" exists

cplusplus {{
#include "EtherFrame_m.h"   // let C++ know about it
}}


packet prpMessage extends EthernetIIFrame
{
    unsigned int sequencenumber; //16Bit
    unsigned int lanIdentifier; // 4Bit
    unsigned int lsdu_size; //12Bit
    
    //4+16+12 = 32 / 8 = 4 Byte extra PRP info
}
