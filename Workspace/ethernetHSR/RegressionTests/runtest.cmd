cls
echo ---
echo --- remove old Testfiles....
echo ---
rm work/work.exe
echo ---
echo --- generate new Testfiles....
echo ---
opp_test -g *.test
echo ---
echo --- generate Makefiles....
echo ---
cd ../src
opp_makemake -f --deep -O out -I../../inet/src/linklayer/ethernet -I../../inet/src/networklayer/rsvp_te -I../../inet/src/networklayer/common -I../../inet/src/networklayer/icmpv6 -I../../inet/src/transport/tcp -I../../inet/src/networklayer/mpls -I../../inet/src/networklayer/ted -I../../inet/src/networklayer/contract -I../../inet/src/util -I../../inet/src/transport/contract -I../../inet/src/linklayer/mfcore -I../../inet/src/networklayer/ldp -I../../inet/src/networklayer/ipv4 -I../../inet/src/base -I../../inet/src/util/headerserializers -I../../inet/src/networklayer/ipv6 -I../../inet/src/transport/sctp -I../../inet/src/world -I../../inet/src/applications/pingapp -I../../inet/src/linklayer/contract -I../../inet/src/networklayer/arp -I../../inet/src/transport/udp -L../../inet/out/gcc-debug/src -lpcap -linet -KINET_PROJ=../../inet --make-lib -o testlib  >> makemakemuell.txt
cd ../RegressionTests

cd work
opp_makemake -I../../src/messages -I../../src/util/testControl -I../../src/util/packer -I../../src -I../../../inet/src/linklayer/ethernet -I../../../inet/src/linklayer/contract -I../../../inet/src/base -L../../../inet/out/gcc-debug/src -linet -L../../out/gcc-debug/src -ltestlib -lpcap -u cmdenv -f --deep -o work  >> makemakemuell.txt
cd ..
echo ---
echo --- make....
echo ---
cd ../src
make >> makeout.txt
cd ../RegressionTests

cd work
make >> makeout.txt
cd ..
echo ---
echo --- run tests....
echo ---
opp_test -r -v -a "-n D:/SVN/hsr/trunk/omnet/inet/src;D:/SVN/hsr/trunk/omnet/ethernetHSR/src;.  " *.test
echo ---
echo ---
echo ---
echo rem cat work/TestCpu/test.out