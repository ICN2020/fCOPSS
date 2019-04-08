INSTDIR=/usr/local
INSTINCDIR=$(INSTDIR)/include
INSTLIBDIR=$(INSTDIR)/lib
INSTBINDIR=$(INSTDIR)/bin

NDNINCDIR=/usr/local/include
NDNLIBDIR=/usr/local/lib

CXX=g++
INCFLAGS=-I. -I../../include
CPPFLAGS=$(INCFLAGS) -I$(NDNINCDIR)
CXXFLAGS=-std=c++14 -Wall -O2 -g
LDFLAGS=-L../lib
LDFLAGS+=-L$(NDNLIBDIR) -L/usr/lib/x86_64-linux-gnu
LDLIBS=-lfcopss -lndn-cxx
LDLIBS+=-lboost_system -lpthread

SRCS=main.cpp
SRCS+=face.cpp
SRCS+=face-manager.cpp
SRCS+=fib-entry.cpp
SRCS+=fib.cpp
SRCS+=st-entry.cpp
SRCS+=st-impl.cpp
SRCS+=forwarder.cpp
SRCS+=transport.cpp
SRCS+=tcp-transport.cpp
SRCS+=udp-transport.cpp
SRCS+=tcp-server.cpp
SRCS+=udp-server.cpp
SRCS+=cmd-server.cpp
SRCS+=router.cpp

OBJS=$(SRCS:%.cpp=%.o)

PROG=router

all: $(PROG)

lib:
	$(MAKE) -e -C ../lib

$(PROG) : lib $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(PROG) $(OBJS) $(LDFLAGS) $(LDLIBS)

clean:
	rm -rf $(PROG) $(OBJS)

install: all
	cp -p $(PROG) $(INSTBINDIR)

uninstall:
	rm -f $(INSTBINDIR)/$(PROG)

depend:
	makedepend -Y -- $(INCFLAGS) -- $(SRCS) >/dev/null 2>&1
	rm -f Makefile.bak

# DO NOT DELETE

main.o: router.hpp ../../include/fcopss/common.hpp
main.o: ../../include/fcopss/config.hpp fib.hpp fib-entry.hpp face.hpp
main.o: ../../include/fcopss/sub.hpp ../../include/fcopss/cd.hpp
main.o: ../../include/fcopss/cd-component.hpp
main.o: ../../include/fcopss/cd-optional.hpp
main.o: ../../include/fcopss/tlv.hpp
main.o: ../../include/fcopss/cd-asterisk.hpp
main.o: ../../include/fcopss/pub-to-rp.hpp ../../include/fcopss/pub.hpp
main.o: ../../include/fcopss/pub-from-rp.hpp transport.hpp st.hpp
main.o: face-manager.hpp forwarder.hpp tcp-server.hpp udp-server.hpp
main.o: cmd-server.hpp
face.o: face.hpp ../../include/fcopss/common.hpp
face.o: ../../include/fcopss/sub.hpp ../../include/fcopss/cd.hpp
face.o: ../../include/fcopss/cd-component.hpp
face.o: ../../include/fcopss/cd-optional.hpp
face.o: ../../include/fcopss/tlv.hpp
face.o: ../../include/fcopss/cd-asterisk.hpp
face.o: ../../include/fcopss/pub-to-rp.hpp ../../include/fcopss/pub.hpp
face.o: ../../include/fcopss/pub-from-rp.hpp transport.hpp
face.o: ../../include/fcopss/log.hpp
face.o: ../../include/fcopss/log-private.hpp
face-manager.o: face-manager.hpp ../../include/fcopss/common.hpp face.hpp
face-manager.o: ../../include/fcopss/sub.hpp
face-manager.o: ../../include/fcopss/cd.hpp
face-manager.o: ../../include/fcopss/cd-component.hpp
face-manager.o: ../../include/fcopss/cd-optional.hpp
face-manager.o: ../../include/fcopss/tlv.hpp
face-manager.o: ../../include/fcopss/cd-asterisk.hpp
face-manager.o: ../../include/fcopss/pub-to-rp.hpp
face-manager.o: ../../include/fcopss/pub.hpp
face-manager.o: ../../include/fcopss/pub-from-rp.hpp transport.hpp
fib-entry.o: fib-entry.hpp ../../include/fcopss/common.hpp face.hpp
fib-entry.o: ../../include/fcopss/sub.hpp ../../include/fcopss/cd.hpp
fib-entry.o: ../../include/fcopss/cd-component.hpp
fib-entry.o: ../../include/fcopss/cd-optional.hpp
fib-entry.o: ../../include/fcopss/tlv.hpp
fib-entry.o: ../../include/fcopss/cd-asterisk.hpp
fib-entry.o: ../../include/fcopss/pub-to-rp.hpp
fib-entry.o: ../../include/fcopss/pub.hpp
fib-entry.o: ../../include/fcopss/pub-from-rp.hpp transport.hpp
fib.o: fib.hpp ../../include/fcopss/common.hpp fib-entry.hpp face.hpp
fib.o: ../../include/fcopss/sub.hpp ../../include/fcopss/cd.hpp
fib.o: ../../include/fcopss/cd-component.hpp
fib.o: ../../include/fcopss/cd-optional.hpp
fib.o: ../../include/fcopss/tlv.hpp
fib.o: ../../include/fcopss/cd-asterisk.hpp
fib.o: ../../include/fcopss/pub-to-rp.hpp ../../include/fcopss/pub.hpp
fib.o: ../../include/fcopss/pub-from-rp.hpp transport.hpp
fib.o: ../../include/fcopss/log.hpp
fib.o: ../../include/fcopss/log-private.hpp
st-entry.o: st-entry.hpp ../../include/fcopss/common.hpp face.hpp
st-entry.o: ../../include/fcopss/sub.hpp ../../include/fcopss/cd.hpp
st-entry.o: ../../include/fcopss/cd-component.hpp
st-entry.o: ../../include/fcopss/cd-optional.hpp
st-entry.o: ../../include/fcopss/tlv.hpp
st-entry.o: ../../include/fcopss/cd-asterisk.hpp
st-entry.o: ../../include/fcopss/pub-to-rp.hpp
st-entry.o: ../../include/fcopss/pub.hpp
st-entry.o: ../../include/fcopss/pub-from-rp.hpp transport.hpp
st-impl.o: st-impl.hpp ../../include/fcopss/common.hpp st.hpp face.hpp
st-impl.o: ../../include/fcopss/sub.hpp ../../include/fcopss/cd.hpp
st-impl.o: ../../include/fcopss/cd-component.hpp
st-impl.o: ../../include/fcopss/cd-optional.hpp
st-impl.o: ../../include/fcopss/tlv.hpp
st-impl.o: ../../include/fcopss/cd-asterisk.hpp
st-impl.o: ../../include/fcopss/pub-to-rp.hpp
st-impl.o: ../../include/fcopss/pub.hpp
st-impl.o: ../../include/fcopss/pub-from-rp.hpp transport.hpp st-entry.hpp
st-impl.o: ../../include/fcopss/log.hpp
st-impl.o: ../../include/fcopss/log-private.hpp
forwarder.o: forwarder.hpp ../../include/fcopss/common.hpp face.hpp
forwarder.o: ../../include/fcopss/sub.hpp ../../include/fcopss/cd.hpp
forwarder.o: ../../include/fcopss/cd-component.hpp
forwarder.o: ../../include/fcopss/cd-optional.hpp
forwarder.o: ../../include/fcopss/tlv.hpp
forwarder.o: ../../include/fcopss/cd-asterisk.hpp
forwarder.o: ../../include/fcopss/pub-to-rp.hpp
forwarder.o: ../../include/fcopss/pub.hpp
forwarder.o: ../../include/fcopss/pub-from-rp.hpp transport.hpp
forwarder.o: face-manager.hpp fib.hpp fib-entry.hpp st.hpp
forwarder.o: ../../include/fcopss/log.hpp
forwarder.o: ../../include/fcopss/log-private.hpp
transport.o: transport.hpp ../../include/fcopss/common.hpp
tcp-transport.o: tcp-transport.hpp transport.hpp
tcp-transport.o: ../../include/fcopss/common.hpp face.hpp
tcp-transport.o: ../../include/fcopss/sub.hpp
tcp-transport.o: ../../include/fcopss/cd.hpp
tcp-transport.o: ../../include/fcopss/cd-component.hpp
tcp-transport.o: ../../include/fcopss/cd-optional.hpp
tcp-transport.o: ../../include/fcopss/tlv.hpp
tcp-transport.o: ../../include/fcopss/cd-asterisk.hpp
tcp-transport.o: ../../include/fcopss/pub-to-rp.hpp
tcp-transport.o: ../../include/fcopss/pub.hpp
tcp-transport.o: ../../include/fcopss/pub-from-rp.hpp
tcp-transport.o: ../../include/fcopss/log.hpp
tcp-transport.o: ../../include/fcopss/log-private.hpp
udp-transport.o: udp-transport.hpp transport.hpp
udp-transport.o: ../../include/fcopss/common.hpp face.hpp
udp-transport.o: ../../include/fcopss/sub.hpp
udp-transport.o: ../../include/fcopss/cd.hpp
udp-transport.o: ../../include/fcopss/cd-component.hpp
udp-transport.o: ../../include/fcopss/cd-optional.hpp
udp-transport.o: ../../include/fcopss/tlv.hpp
udp-transport.o: ../../include/fcopss/cd-asterisk.hpp
udp-transport.o: ../../include/fcopss/pub-to-rp.hpp
udp-transport.o: ../../include/fcopss/pub.hpp
udp-transport.o: ../../include/fcopss/pub-from-rp.hpp
udp-transport.o: ../../include/fcopss/log.hpp
udp-transport.o: ../../include/fcopss/log-private.hpp
tcp-server.o: tcp-server.hpp ../../include/fcopss/common.hpp
tcp-server.o: tcp-transport.hpp transport.hpp face-manager.hpp face.hpp
tcp-server.o: ../../include/fcopss/sub.hpp ../../include/fcopss/cd.hpp
tcp-server.o: ../../include/fcopss/cd-component.hpp
tcp-server.o: ../../include/fcopss/cd-optional.hpp
tcp-server.o: ../../include/fcopss/tlv.hpp
tcp-server.o: ../../include/fcopss/cd-asterisk.hpp
tcp-server.o: ../../include/fcopss/pub-to-rp.hpp
tcp-server.o: ../../include/fcopss/pub.hpp
tcp-server.o: ../../include/fcopss/pub-from-rp.hpp
tcp-server.o: ../../include/fcopss/log.hpp
tcp-server.o: ../../include/fcopss/log-private.hpp
udp-server.o: udp-server.hpp ../../include/fcopss/common.hpp
udp-server.o: udp-transport.hpp transport.hpp face-manager.hpp face.hpp
udp-server.o: ../../include/fcopss/sub.hpp ../../include/fcopss/cd.hpp
udp-server.o: ../../include/fcopss/cd-component.hpp
udp-server.o: ../../include/fcopss/cd-optional.hpp
udp-server.o: ../../include/fcopss/tlv.hpp
udp-server.o: ../../include/fcopss/cd-asterisk.hpp
udp-server.o: ../../include/fcopss/pub-to-rp.hpp
udp-server.o: ../../include/fcopss/pub.hpp
udp-server.o: ../../include/fcopss/pub-from-rp.hpp
udp-server.o: ../../include/fcopss/log.hpp
udp-server.o: ../../include/fcopss/log-private.hpp
cmd-server.o: cmd-server.hpp ../../include/fcopss/common.hpp face.hpp
cmd-server.o: ../../include/fcopss/sub.hpp ../../include/fcopss/cd.hpp
cmd-server.o: ../../include/fcopss/cd-component.hpp
cmd-server.o: ../../include/fcopss/cd-optional.hpp
cmd-server.o: ../../include/fcopss/tlv.hpp
cmd-server.o: ../../include/fcopss/cd-asterisk.hpp
cmd-server.o: ../../include/fcopss/pub-to-rp.hpp
cmd-server.o: ../../include/fcopss/pub.hpp
cmd-server.o: ../../include/fcopss/pub-from-rp.hpp transport.hpp
cmd-server.o: tcp-transport.hpp udp-transport.hpp face-manager.hpp fib.hpp
cmd-server.o: fib-entry.hpp st.hpp ../../include/fcopss/log.hpp
cmd-server.o: ../../include/fcopss/log-private.hpp
router.o: router.hpp ../../include/fcopss/common.hpp
router.o: ../../include/fcopss/config.hpp fib.hpp fib-entry.hpp face.hpp
router.o: ../../include/fcopss/sub.hpp ../../include/fcopss/cd.hpp
router.o: ../../include/fcopss/cd-component.hpp
router.o: ../../include/fcopss/cd-optional.hpp
router.o: ../../include/fcopss/tlv.hpp
router.o: ../../include/fcopss/cd-asterisk.hpp
router.o: ../../include/fcopss/pub-to-rp.hpp
router.o: ../../include/fcopss/pub.hpp
router.o: ../../include/fcopss/pub-from-rp.hpp transport.hpp st.hpp
router.o: face-manager.hpp forwarder.hpp tcp-server.hpp udp-server.hpp
router.o: cmd-server.hpp st-impl.hpp st-entry.hpp
