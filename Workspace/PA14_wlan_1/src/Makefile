#
# OMNeT++/OMNEST Makefile for PA14_wlan_1
#
# This file was generated with the command:
#  opp_makemake -f --deep -I../../inet/src/linklayer/ethernet -I../../inet/src/linklayer/ieee80211/radio -I../../inet/src/networklayer/routing/aodv -I../../inet/src/networklayer/common -I../../inet/src/networklayer/icmpv6 -I../../inet/src -I../../inet/src/world/obstacles -I../../inet/src/networklayer/xmipv6 -I../../inet/src/networklayer/contract -I../../inet/src/networklayer/autorouting/ipv4 -I../../inet/src/util -I../../inet/src/transport/contract -I../../inet/src/linklayer/common -I../../inet/src/status -I../../inet/src/linklayer/radio/propagation -I../../inet/src/linklayer/ieee80211/radio/errormodel -I../../inet/src/linklayer/radio -I../../inet/src/util/headerserializers/tcp -I../../inet/src/networklayer/ipv4 -I../../inet/src/mobility/contract -I../../inet/src/util/headerserializers/ipv4 -I../../inet/src/base -I../../inet/src/util/headerserializers -I../../inet/src/world/radio -I../../inet/src/linklayer/ieee80211/mac -I../../inet/src/transport/sctp -I../../inet/src/networklayer/ipv6 -I../../inet/src/util/headerserializers/udp -I../../inet/src/networklayer/ipv6tunneling -I../../inet/src/applications/pingapp -I../../inet/src/battery/models -I../../inet/src/util/headerserializers/ipv6 -I../../inet/src/util/headerserializers/sctp -I../../inet/src/linklayer/contract -I../../inet/src/networklayer/arp -I../../inet/src/transport/tcp_common -I../../inet/src/transport/udp -L../../inet/out/$$\(CONFIGNAME\)/src -linet -DINET_IMPORT -KINET_PROJ=../../inet
#

# Name of target to be created (-o option)
TARGET = PA14_wlan_1$(EXE_SUFFIX)

# User interface (uncomment one) (-u option)
USERIF_LIBS = $(ALL_ENV_LIBS) # that is, $(TKENV_LIBS) $(CMDENV_LIBS)
#USERIF_LIBS = $(CMDENV_LIBS)
#USERIF_LIBS = $(TKENV_LIBS)

# C++ include paths (with -I)
INCLUDE_PATH = \
    -I../../inet/src/linklayer/ethernet \
    -I../../inet/src/linklayer/ieee80211/radio \
    -I../../inet/src/networklayer/routing/aodv \
    -I../../inet/src/networklayer/common \
    -I../../inet/src/networklayer/icmpv6 \
    -I../../inet/src \
    -I../../inet/src/world/obstacles \
    -I../../inet/src/networklayer/xmipv6 \
    -I../../inet/src/networklayer/contract \
    -I../../inet/src/networklayer/autorouting/ipv4 \
    -I../../inet/src/util \
    -I../../inet/src/transport/contract \
    -I../../inet/src/linklayer/common \
    -I../../inet/src/status \
    -I../../inet/src/linklayer/radio/propagation \
    -I../../inet/src/linklayer/ieee80211/radio/errormodel \
    -I../../inet/src/linklayer/radio \
    -I../../inet/src/util/headerserializers/tcp \
    -I../../inet/src/networklayer/ipv4 \
    -I../../inet/src/mobility/contract \
    -I../../inet/src/util/headerserializers/ipv4 \
    -I../../inet/src/base \
    -I../../inet/src/util/headerserializers \
    -I../../inet/src/world/radio \
    -I../../inet/src/linklayer/ieee80211/mac \
    -I../../inet/src/transport/sctp \
    -I../../inet/src/networklayer/ipv6 \
    -I../../inet/src/util/headerserializers/udp \
    -I../../inet/src/networklayer/ipv6tunneling \
    -I../../inet/src/applications/pingapp \
    -I../../inet/src/battery/models \
    -I../../inet/src/util/headerserializers/ipv6 \
    -I../../inet/src/util/headerserializers/sctp \
    -I../../inet/src/linklayer/contract \
    -I../../inet/src/networklayer/arp \
    -I../../inet/src/transport/tcp_common \
    -I../../inet/src/transport/udp \
    -I. \
    -Ichannels \
    -Icpu \
    -IduplicateDetection \
    -IendDevices \
    -IforwardDelay \
    -Imac \
    -Imessages \
    -InodeTable \
    -Ipacker \
    -Iswitch \
    -ItestControl

# Additional object and library files to link with
EXTRA_OBJS =

# Additional libraries (-L, -l options)
LIBS = -L../../inet/out/$(CONFIGNAME)/src  -linet
LIBS += -Wl,-rpath,`abspath ../../inet/out/$(CONFIGNAME)/src`

# Output directory
PROJECT_OUTPUT_DIR = ../out
PROJECTRELATIVE_PATH = src
O = $(PROJECT_OUTPUT_DIR)/$(CONFIGNAME)/$(PROJECTRELATIVE_PATH)

# Object files for local .cc and .msg files
OBJS = \
    $O/cpu/CPU.o \
    $O/duplicateDetection/ListenErkennung.o \
    $O/duplicateDetection/keyMSG.o \
    $O/duplicateDetection/MessageList.o \
    $O/forwardDelay/ForwardDelay.o \
    $O/mac/ModifiedEtherMACBase.o \
    $O/nodeTable/nodeTable.o \
    $O/packer/DecapsulateMessage.o \
    $O/packer/EncapsulateMessage.o \
    $O/packer/MessagePacker.o \
    $O/switch/EndNodeSwitch.o \
    $O/switch/RedBoxSwitch.o \
    $O/testControl/TestControl.o \
    $O/messages/vlanMessage_m.o \
    $O/messages/hsrMessage_m.o \
    $O/messages/dataMessage_m.o \
    $O/messages/prpMessage_m.o \
    $O/messages/cpuSelfMessage_m.o \
    $O/nodeTable/nodeTableTimeoutMessage_m.o

# Message files
MSGFILES = \
    messages/vlanMessage.msg \
    messages/hsrMessage.msg \
    messages/dataMessage.msg \
    messages/prpMessage.msg \
    messages/cpuSelfMessage.msg \
    nodeTable/nodeTableTimeoutMessage.msg

# Other makefile variables (-K)
INET_PROJ=../../inet

#------------------------------------------------------------------------------

# Pull in OMNeT++ configuration (Makefile.inc or configuser.vc)

ifneq ("$(OMNETPP_CONFIGFILE)","")
CONFIGFILE = $(OMNETPP_CONFIGFILE)
else
ifneq ("$(OMNETPP_ROOT)","")
CONFIGFILE = $(OMNETPP_ROOT)/Makefile.inc
else
CONFIGFILE = $(shell opp_configfilepath)
endif
endif

ifeq ("$(wildcard $(CONFIGFILE))","")
$(error Config file '$(CONFIGFILE)' does not exist -- add the OMNeT++ bin directory to the path so that opp_configfilepath can be found, or set the OMNETPP_CONFIGFILE variable to point to Makefile.inc)
endif

include $(CONFIGFILE)

# Simulation kernel and user interface libraries
OMNETPP_LIB_SUBDIR = $(OMNETPP_LIB_DIR)/$(TOOLCHAIN_NAME)
OMNETPP_LIBS = -L"$(OMNETPP_LIB_SUBDIR)" -L"$(OMNETPP_LIB_DIR)" -loppmain$D $(USERIF_LIBS) $(KERNEL_LIBS) $(SYS_LIBS)

COPTS = $(CFLAGS) -DINET_IMPORT $(INCLUDE_PATH) -I$(OMNETPP_INCL_DIR)
MSGCOPTS = $(INCLUDE_PATH)

# we want to recompile everything if COPTS changes,
# so we store COPTS into $COPTS_FILE and have object
# files depend on it (except when "make depend" was called)
COPTS_FILE = $O/.last-copts
ifneq ($(MAKECMDGOALS),depend)
ifneq ("$(COPTS)","$(shell cat $(COPTS_FILE) 2>/dev/null || echo '')")
$(shell $(MKPATH) "$O" && echo "$(COPTS)" >$(COPTS_FILE))
endif
endif

#------------------------------------------------------------------------------
# User-supplied makefile fragment(s)
# >>>
# <<<
#------------------------------------------------------------------------------

# Main target
all: $O/$(TARGET)
	$(Q)$(LN) $O/$(TARGET) .

$O/$(TARGET): $(OBJS)  $(wildcard $(EXTRA_OBJS)) Makefile
	@$(MKPATH) $O
	@echo Creating executable: $@
	$(Q)$(CXX) $(LDFLAGS) -o $O/$(TARGET)  $(OBJS) $(EXTRA_OBJS) $(AS_NEEDED_OFF) $(WHOLE_ARCHIVE_ON) $(LIBS) $(WHOLE_ARCHIVE_OFF) $(OMNETPP_LIBS)

.PHONY: all clean cleanall depend msgheaders

.SUFFIXES: .cc

$O/%.o: %.cc $(COPTS_FILE)
	@$(MKPATH) $(dir $@)
	$(qecho) "$<"
	$(Q)$(CXX) -c $(CXXFLAGS) $(COPTS) -o $@ $<

%_m.cc %_m.h: %.msg
	$(qecho) MSGC: $<
	$(Q)$(MSGC) -s _m.cc $(MSGCOPTS) $?

msgheaders: $(MSGFILES:.msg=_m.h)

clean:
	$(qecho) Cleaning...
	$(Q)-rm -rf $O
	$(Q)-rm -f PA14_wlan_1 PA14_wlan_1.exe libPA14_wlan_1.so libPA14_wlan_1.a libPA14_wlan_1.dll libPA14_wlan_1.dylib
	$(Q)-rm -f ./*_m.cc ./*_m.h
	$(Q)-rm -f channels/*_m.cc channels/*_m.h
	$(Q)-rm -f cpu/*_m.cc cpu/*_m.h
	$(Q)-rm -f duplicateDetection/*_m.cc duplicateDetection/*_m.h
	$(Q)-rm -f endDevices/*_m.cc endDevices/*_m.h
	$(Q)-rm -f forwardDelay/*_m.cc forwardDelay/*_m.h
	$(Q)-rm -f mac/*_m.cc mac/*_m.h
	$(Q)-rm -f messages/*_m.cc messages/*_m.h
	$(Q)-rm -f nodeTable/*_m.cc nodeTable/*_m.h
	$(Q)-rm -f packer/*_m.cc packer/*_m.h
	$(Q)-rm -f switch/*_m.cc switch/*_m.h
	$(Q)-rm -f testControl/*_m.cc testControl/*_m.h

cleanall: clean
	$(Q)-rm -rf $(PROJECT_OUTPUT_DIR)

depend:
	$(qecho) Creating dependencies...
	$(Q)$(MAKEDEPEND) $(INCLUDE_PATH) -f Makefile -P\$$O/ -- $(MSG_CC_FILES)  ./*.cc channels/*.cc cpu/*.cc duplicateDetection/*.cc endDevices/*.cc forwardDelay/*.cc mac/*.cc messages/*.cc nodeTable/*.cc packer/*.cc switch/*.cc testControl/*.cc

# DO NOT DELETE THIS LINE -- make depend depends on it.
$O/cpu/CPU.o: cpu/CPU.cc \
	cpu/CPU.h \
	hsrDefines.h \
	messages/cpuSelfMessage_m.h \
	messages/dataMessage_m.h \
	messages/hsrMessage_m.h \
	messages/prpMessage_m.h \
	messages/vlanMessage_m.h \
	packer/MessagePacker.h \
	testControl/TestControl.h \
	$(INET_PROJ)/src/base/Compat.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/linklayer/contract/Ieee802Ctrl_m.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/linklayer/ethernet/EtherFrame_m.h \
	$(INET_PROJ)/src/linklayer/ethernet/Ethernet.h
$O/duplicateDetection/ListenErkennung.o: duplicateDetection/ListenErkennung.cc \
	duplicateDetection/ListenErkennung.h \
	duplicateDetection/MessageList.h \
	duplicateDetection/keyMSG.h \
	hsrDefines.h \
	messages/dataMessage_m.h \
	messages/hsrMessage_m.h \
	messages/prpMessage_m.h \
	messages/vlanMessage_m.h \
	packer/MessagePacker.h \
	testControl/TestControl.h \
	$(INET_PROJ)/src/base/Compat.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/linklayer/contract/Ieee802Ctrl_m.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/linklayer/ethernet/EtherFrame_m.h \
	$(INET_PROJ)/src/linklayer/ethernet/Ethernet.h
$O/duplicateDetection/MessageList.o: duplicateDetection/MessageList.cc \
	duplicateDetection/MessageList.h \
	duplicateDetection/keyMSG.h \
	messages/hsrMessage_m.h \
	$(INET_PROJ)/src/base/Compat.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h
$O/duplicateDetection/keyMSG.o: duplicateDetection/keyMSG.cc \
	duplicateDetection/keyMSG.h
$O/forwardDelay/ForwardDelay.o: forwardDelay/ForwardDelay.cc \
	forwardDelay/ForwardDelay.h
$O/mac/ModifiedEtherMACBase.o: mac/ModifiedEtherMACBase.cc \
	duplicateDetection/ListenErkennung.h \
	duplicateDetection/MessageList.h \
	duplicateDetection/keyMSG.h \
	hsrDefines.h \
	mac/ModifiedEtherMACBase.h \
	messages/dataMessage_m.h \
	messages/hsrMessage_m.h \
	messages/prpMessage_m.h \
	messages/vlanMessage_m.h \
	packer/MessagePacker.h \
	testControl/TestControl.h \
	$(INET_PROJ)/src/base/Compat.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/INotifiable.h \
	$(INET_PROJ)/src/base/IPassiveQueue.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/base/NotificationBoard.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	$(INET_PROJ)/src/linklayer/contract/Ieee802Ctrl_m.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/linklayer/contract/TxNotifDetails.h \
	$(INET_PROJ)/src/linklayer/ethernet/EtherFrame_m.h \
	$(INET_PROJ)/src/linklayer/ethernet/Ethernet.h \
	$(INET_PROJ)/src/networklayer/common/IInterfaceTable.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceEntry.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceTableAccess.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceToken.h \
	$(INET_PROJ)/src/networklayer/contract/IPv4Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h
$O/messages/cpuSelfMessage_m.o: messages/cpuSelfMessage_m.cc \
	hsrDefines.h \
	messages/cpuSelfMessage_m.h \
	$(INET_PROJ)/src/base/Compat.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h
$O/messages/dataMessage_m.o: messages/dataMessage_m.cc \
	messages/dataMessage_m.h
$O/messages/hsrMessage_m.o: messages/hsrMessage_m.cc \
	messages/hsrMessage_m.h
$O/messages/prpMessage_m.o: messages/prpMessage_m.cc \
	messages/prpMessage_m.h \
	$(INET_PROJ)/src/base/Compat.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/linklayer/contract/Ieee802Ctrl_m.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/linklayer/ethernet/EtherFrame_m.h \
	$(INET_PROJ)/src/linklayer/ethernet/Ethernet.h
$O/messages/vlanMessage_m.o: messages/vlanMessage_m.cc \
	messages/vlanMessage_m.h
$O/nodeTable/nodeTable.o: nodeTable/nodeTable.cc \
	nodeTable/nodeTable.h \
	nodeTable/nodeTableTimeoutMessage_m.h \
	$(INET_PROJ)/src/base/Compat.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/linklayer/contract/Ieee802Ctrl_m.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/linklayer/ethernet/Ethernet.h
$O/nodeTable/nodeTableTimeoutMessage_m.o: nodeTable/nodeTableTimeoutMessage_m.cc \
	nodeTable/nodeTableTimeoutMessage_m.h \
	$(INET_PROJ)/src/base/Compat.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/linklayer/contract/Ieee802Ctrl_m.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/linklayer/ethernet/Ethernet.h
$O/packer/DecapsulateMessage.o: packer/DecapsulateMessage.cc \
	messages/dataMessage_m.h \
	messages/hsrMessage_m.h \
	messages/prpMessage_m.h \
	messages/vlanMessage_m.h \
	packer/DecapsulateMessage.h \
	packer/MessagePacker.h \
	$(INET_PROJ)/src/base/Compat.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/linklayer/contract/Ieee802Ctrl_m.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/linklayer/ethernet/EtherFrame_m.h \
	$(INET_PROJ)/src/linklayer/ethernet/Ethernet.h
$O/packer/EncapsulateMessage.o: packer/EncapsulateMessage.cc \
	messages/dataMessage_m.h \
	messages/hsrMessage_m.h \
	messages/prpMessage_m.h \
	messages/vlanMessage_m.h \
	packer/EncapsulateMessage.h \
	packer/MessagePacker.h \
	$(INET_PROJ)/src/base/Compat.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/linklayer/contract/Ieee802Ctrl_m.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/linklayer/ethernet/EtherFrame_m.h \
	$(INET_PROJ)/src/linklayer/ethernet/Ethernet.h
$O/packer/MessagePacker.o: packer/MessagePacker.cc \
	messages/dataMessage_m.h \
	messages/hsrMessage_m.h \
	messages/prpMessage_m.h \
	messages/vlanMessage_m.h \
	packer/MessagePacker.h \
	$(INET_PROJ)/src/base/Compat.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/linklayer/contract/Ieee802Ctrl_m.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/linklayer/ethernet/EtherFrame_m.h \
	$(INET_PROJ)/src/linklayer/ethernet/Ethernet.h
$O/switch/EndNodeSwitch.o: switch/EndNodeSwitch.cc \
	duplicateDetection/ListenErkennung.h \
	duplicateDetection/MessageList.h \
	duplicateDetection/keyMSG.h \
	hsrDefines.h \
	messages/dataMessage_m.h \
	messages/hsrMessage_m.h \
	messages/prpMessage_m.h \
	messages/vlanMessage_m.h \
	nodeTable/nodeTable.h \
	packer/MessagePacker.h \
	switch/EndNodeSwitch.h \
	testControl/TestControl.h \
	$(INET_PROJ)/src/base/Compat.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/linklayer/contract/Ieee802Ctrl_m.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/linklayer/ethernet/EtherFrame_m.h \
	$(INET_PROJ)/src/linklayer/ethernet/Ethernet.h
$O/switch/RedBoxSwitch.o: switch/RedBoxSwitch.cc \
	messages/dataMessage_m.h \
	messages/hsrMessage_m.h \
	messages/prpMessage_m.h \
	messages/vlanMessage_m.h \
	packer/MessagePacker.h \
	switch/RedBoxSwitch.h \
	$(INET_PROJ)/src/base/Compat.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/linklayer/contract/Ieee802Ctrl_m.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/linklayer/ethernet/EtherFrame_m.h \
	$(INET_PROJ)/src/linklayer/ethernet/Ethernet.h
$O/testControl/TestControl.o: testControl/TestControl.cc \
	hsrDefines.h \
	messages/dataMessage_m.h \
	messages/hsrMessage_m.h \
	messages/prpMessage_m.h \
	messages/vlanMessage_m.h \
	packer/MessagePacker.h \
	testControl/TestControl.h \
	$(INET_PROJ)/src/base/Compat.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/linklayer/contract/Ieee802Ctrl_m.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/linklayer/ethernet/EtherFrame_m.h \
	$(INET_PROJ)/src/linklayer/ethernet/Ethernet.h

