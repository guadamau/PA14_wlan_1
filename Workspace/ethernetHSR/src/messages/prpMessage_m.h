//
// Generated file, do not edit! Created by opp_msgc 4.5 from messages/prpMessage.msg.
//

#ifndef _PRPMESSAGE_M_H_
#define _PRPMESSAGE_M_H_

#include <omnetpp.h>

// opp_msgc version check
#define MSGC_VERSION 0x0405
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of opp_msgc: 'make clean' should help.
#endif

// cplusplus {{
#include "EtherFrame_m.h"
// }}



/**
 * Class generated from <tt>messages/prpMessage.msg</tt> by opp_msgc.
 * <pre>
 * packet prpMessage extends EthernetIIFrame
 * {
 *     unsigned int sequencenumber; 
 *     unsigned int lanIdentifier; 
 *     unsigned int lsdu_size; 
 *     
 *     
 * }
 * </pre>
 */
class prpMessage : public ::EthernetIIFrame
{
  protected:
    unsigned int sequencenumber_var;
    unsigned int lanIdentifier_var;
    unsigned int lsdu_size_var;

  private:
    void copy(const prpMessage& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const prpMessage&);

  public:
    prpMessage(const char *name=NULL, int kind=0);
    prpMessage(const prpMessage& other);
    virtual ~prpMessage();
    prpMessage& operator=(const prpMessage& other);
    virtual prpMessage *dup() const {return new prpMessage(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual unsigned int getSequencenumber() const;
    virtual void setSequencenumber(unsigned int sequencenumber);
    virtual unsigned int getLanIdentifier() const;
    virtual void setLanIdentifier(unsigned int lanIdentifier);
    virtual unsigned int getLsdu_size() const;
    virtual void setLsdu_size(unsigned int lsdu_size);
};

inline void doPacking(cCommBuffer *b, prpMessage& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, prpMessage& obj) {obj.parsimUnpack(b);}


#endif // _PRPMESSAGE_M_H_
