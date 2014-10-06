//
// Generated file, do not edit! Created by opp_msgc 4.5 from messages/dataMessage.msg.
//

#ifndef _DATAMESSAGE_M_H_
#define _DATAMESSAGE_M_H_

#include <omnetpp.h>

// opp_msgc version check
#define MSGC_VERSION 0x0405
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of opp_msgc: 'make clean' should help.
#endif



/**
 * Class generated from <tt>messages/dataMessage.msg</tt> by opp_msgc.
 * <pre>
 * packet dataMessage
 * {
 * 	unsigned long counter;
 * 	double sendTime;
 * 	char randomData[500];
 * 	
 *     int ownEtherType = 0x0800; 
 * }
 * </pre>
 */
class dataMessage : public ::cPacket
{
  protected:
    unsigned long counter_var;
    double sendTime_var;
    char randomData_var[500];
    int ownEtherType_var;

  private:
    void copy(const dataMessage& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const dataMessage&);

  public:
    dataMessage(const char *name=NULL, int kind=0);
    dataMessage(const dataMessage& other);
    virtual ~dataMessage();
    dataMessage& operator=(const dataMessage& other);
    virtual dataMessage *dup() const {return new dataMessage(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual unsigned long getCounter() const;
    virtual void setCounter(unsigned long counter);
    virtual double getSendTime() const;
    virtual void setSendTime(double sendTime);
    virtual unsigned int getRandomDataArraySize() const;
    virtual char getRandomData(unsigned int k) const;
    virtual void setRandomData(unsigned int k, char randomData);
    virtual int getOwnEtherType() const;
    virtual void setOwnEtherType(int ownEtherType);
};

inline void doPacking(cCommBuffer *b, dataMessage& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, dataMessage& obj) {obj.parsimUnpack(b);}


#endif // _DATAMESSAGE_M_H_
