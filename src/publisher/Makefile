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

SRCS=publish.cpp

OBJS=$(SRCS:%.cpp=%.o)

PROG=publish

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

publish.o: ../../include/fcopss/face.hpp ../../include/fcopss/common.hpp
publish.o: ../../include/fcopss/sub.hpp ../../include/fcopss/cd.hpp
publish.o: ../../include/fcopss/cd-component.hpp
publish.o: ../../include/fcopss/cd-optional.hpp ../../include/fcopss/tlv.hpp
publish.o: ../../include/fcopss/cd-asterisk.hpp
publish.o: ../../include/fcopss/pub-from-rp.hpp ../../include/fcopss/pub.hpp
publish.o: ../../include/fcopss/pub-to-rp.hpp
