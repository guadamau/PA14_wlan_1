//
// Generated file, do not edit! Created by opp_msgc 4.5 from messages/hsrSwitchSelfMessage.msg.
//

#ifndef _HSRSWITCHSELFMESSAGE_M_H_
#define _HSRSWITCHSELFMESSAGE_M_H_

#include <omnetpp.h>

// opp_msgc version check
#define MSGC_VERSION 0x0405
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of opp_msgc: 'make clean' should help.
#endif



/**
 * Class generated from <tt>messages/hsrSwitchSelfMessage.msg</tt> by opp_msgc.
 * <pre>
 * message HsrSwitchSelfMessage {
 *     unsigned char schedulerName; 
 * }
 * </pre>
 */
class HsrSwitchSelfMessage : public ::cMessage
{
  protected:
    unsigned char schedulerName_var;

  private:
    void copy(const HsrSwitchSelfMessage& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const HsrSwitchSelfMessage&);

  public:
    HsrSwitchSelfMessage(const char *name=NULL, int kind=0);
    HsrSwitchSelfMessage(const HsrSwitchSelfMessage& other);
    virtual ~HsrSwitchSelfMessage();
    HsrSwitchSelfMessage& operator=(const HsrSwitchSelfMessage& other);
    virtual HsrSwitchSelfMessage *dup() const {return new HsrSwitchSelfMessage(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual unsigned char getSchedulerName() const;
    virtual void setSchedulerName(unsigned char schedulerName);
};

inline void doPacking(cCommBuffer *b, HsrSwitchSelfMessage& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, HsrSwitchSelfMessage& obj) {obj.parsimUnpack(b);}


#endif // _HSRSWITCHSELFMESSAGE_M_H_
