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

SRCS=subscribe.cpp

OBJS=$(SRCS:%.cpp=%.o)

PROG=subscribe

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

subscribe.o: ../../include/fcopss/face.hpp ../../include/fcopss/common.hpp
subscribe.o: ../../include/fcopss/sub.hpp ../../include/fcopss/cd.hpp
subscribe.o: ../../include/fcopss/cd-component.hpp
subscribe.o: ../../include/fcopss/cd-optional.hpp
subscribe.o: ../../include/fcopss/tlv.hpp
subscribe.o: ../../include/fcopss/cd-asterisk.hpp
subscribe.o: ../../include/fcopss/pub-from-rp.hpp
subscribe.o: ../../include/fcopss/pub.hpp ../../include/fcopss/pub-to-rp.hpp
