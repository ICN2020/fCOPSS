INSTDIR=/usr/local
INSTINCDIR=$(INSTDIR)/include
INSTLIBDIR=$(INSTDIR)/lib
INSTBINDIR=$(INSTDIR)/bin

NDNINCS=$(shell pkg-config --cflags-only-I libndn-cxx)
NDNCXXF=$(shell pkg-config pkg-config --cflags-only-other libndn-cxx)
NDNLIBS=$(shell pkg-config --libs libndn-cxx)

CXX=g++
INCFLAGS=-I. -I../../include
CPPFLAGS=$(INCFLAGS) $(NDNINCS)
CXXFLAGS=-std=c++14 -Wall -O2 -g $(NDNCXXF)
LDFLAGS=-L/usr/lib/x86_64-linux-gnu -L../lib
LDLIBS=-lfcopss
LDLIBS+=$(NDNLIBS)

SRCS=main.cpp
SRCS+=face.cpp
SRCS+=face-manager.cpp
SRCS+=forwarder.cpp
SRCS+=transport.cpp
SRCS+=tcp-transport.cpp
SRCS+=udp-transport.cpp
SRCS+=tcp-server.cpp
SRCS+=udp-server.cpp
SRCS+=rendezvous.cpp

OBJS=$(SRCS:%.cpp=%.o)

PROG=rendezvous

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

main.o: rendezvous.hpp ../../include/fcopss/common.hpp
main.o: ../../include/fcopss/config.hpp face-manager.hpp face.hpp
main.o: ../../include/fcopss/sub.hpp ../../include/fcopss/cd.hpp
main.o: ../../include/fcopss/cd-component.hpp
main.o: ../../include/fcopss/cd-optional.hpp ../../include/fcopss/tlv.hpp
main.o: ../../include/fcopss/cd-asterisk.hpp
main.o: ../../include/fcopss/pub-to-rp.hpp ../../include/fcopss/pub.hpp
main.o: ../../include/fcopss/pub-from-rp.hpp transport.hpp forwarder.hpp
main.o: tcp-server.hpp udp-server.hpp
face.o: face.hpp ../../include/fcopss/common.hpp ../../include/fcopss/sub.hpp
face.o: ../../include/fcopss/cd.hpp ../../include/fcopss/cd-component.hpp
face.o: ../../include/fcopss/cd-optional.hpp ../../include/fcopss/tlv.hpp
face.o: ../../include/fcopss/cd-asterisk.hpp
face.o: ../../include/fcopss/pub-to-rp.hpp ../../include/fcopss/pub.hpp
face.o: ../../include/fcopss/pub-from-rp.hpp transport.hpp
face.o: ../../include/fcopss/log.hpp ../../include/fcopss/log-private.hpp
face-manager.o: face-manager.hpp ../../include/fcopss/common.hpp face.hpp
face-manager.o: ../../include/fcopss/sub.hpp ../../include/fcopss/cd.hpp
face-manager.o: ../../include/fcopss/cd-component.hpp
face-manager.o: ../../include/fcopss/cd-optional.hpp
face-manager.o: ../../include/fcopss/tlv.hpp
face-manager.o: ../../include/fcopss/cd-asterisk.hpp
face-manager.o: ../../include/fcopss/pub-to-rp.hpp
face-manager.o: ../../include/fcopss/pub.hpp
face-manager.o: ../../include/fcopss/pub-from-rp.hpp transport.hpp
face-manager.o: ../../include/fcopss/log.hpp
face-manager.o: ../../include/fcopss/log-private.hpp
forwarder.o: forwarder.hpp ../../include/fcopss/common.hpp face.hpp
forwarder.o: ../../include/fcopss/sub.hpp ../../include/fcopss/cd.hpp
forwarder.o: ../../include/fcopss/cd-component.hpp
forwarder.o: ../../include/fcopss/cd-optional.hpp
forwarder.o: ../../include/fcopss/tlv.hpp
forwarder.o: ../../include/fcopss/cd-asterisk.hpp
forwarder.o: ../../include/fcopss/pub-to-rp.hpp ../../include/fcopss/pub.hpp
forwarder.o: ../../include/fcopss/pub-from-rp.hpp transport.hpp
forwarder.o: face-manager.hpp ../../include/fcopss/log.hpp
forwarder.o: ../../include/fcopss/log-private.hpp
transport.o: transport.hpp ../../include/fcopss/common.hpp
tcp-transport.o: tcp-transport.hpp transport.hpp
tcp-transport.o: ../../include/fcopss/common.hpp face.hpp
tcp-transport.o: ../../include/fcopss/sub.hpp ../../include/fcopss/cd.hpp
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
udp-transport.o: ../../include/fcopss/sub.hpp ../../include/fcopss/cd.hpp
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
tcp-server.o: ../../include/fcopss/pub-to-rp.hpp ../../include/fcopss/pub.hpp
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
udp-server.o: ../../include/fcopss/pub-to-rp.hpp ../../include/fcopss/pub.hpp
udp-server.o: ../../include/fcopss/pub-from-rp.hpp
udp-server.o: ../../include/fcopss/log.hpp
udp-server.o: ../../include/fcopss/log-private.hpp
rendezvous.o: rendezvous.hpp ../../include/fcopss/common.hpp
rendezvous.o: ../../include/fcopss/config.hpp face-manager.hpp face.hpp
rendezvous.o: ../../include/fcopss/sub.hpp ../../include/fcopss/cd.hpp
rendezvous.o: ../../include/fcopss/cd-component.hpp
rendezvous.o: ../../include/fcopss/cd-optional.hpp
rendezvous.o: ../../include/fcopss/tlv.hpp
rendezvous.o: ../../include/fcopss/cd-asterisk.hpp
rendezvous.o: ../../include/fcopss/pub-to-rp.hpp ../../include/fcopss/pub.hpp
rendezvous.o: ../../include/fcopss/pub-from-rp.hpp transport.hpp
rendezvous.o: forwarder.hpp tcp-server.hpp udp-server.hpp
rendezvous.o: ../../include/fcopss/log.hpp
rendezvous.o: ../../include/fcopss/log-private.hpp
