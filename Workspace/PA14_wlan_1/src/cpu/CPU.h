#ifndef __CPU_H__
#define __CPU_H__

#include <omnetpp.h>
#include "cpuSelfMessage_m.h"
#include "hsrDefines.h"
#include "EtherFrame_m.h"
#include "vlanMessage_m.h"

#include "DelayLogger.h"

/**
 * TODO - Generated class
 */
class CPU : public cSimpleModule
{
private:
  MACAddress macAddress;

  cGate* gateIn;
  cGate* gateInExp;
  cGate* gateOut;
  cGate* gateOutExp;

  cXMLElement* rootelement;

  unsigned int sequenceNumber;

  int multicastListener;

  static unsigned long messageCount;

  DelayLogger* delayLogger;

  EthernetIIFrame *generateOnePacket(SendData sendData);
  bool scheduleMessage(SendData sendData);
  void loadXMLFile();
  void printMessageinfo(SendData sendData);

protected:
  //cOutVector endToEndDelayVec[10];

  // statistics
  unsigned long numFramesSent;
  unsigned long numFramesReceived;  

  virtual void initialize();
  virtual void handleMessage( cMessage *msg );
  virtual void finish();

public:
  CPU();
  virtual ~CPU();
};

#endif
