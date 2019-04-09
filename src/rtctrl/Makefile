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

SRCS= main.cpp
SRCS+= cmd-fibadd.cpp
SRCS+= cmd-fibdel.cpp
SRCS+= cmd-fibdump.cpp
SRCS+= cmd-stdel.cpp
SRCS+= cmd-stdump.cpp
SRCS+= cmd-facedel.cpp
SRCS+= cmd-facedump.cpp
SRCS+= cmd.cpp
SRCS+= request.cpp
SRCS+= response.cpp
SRCS+= rtctrl.cpp

OBJS=$(SRCS:%.cpp=%.o)

PROG=rtctrl

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

main.o: rtctrl.hpp ../../include/fcopss/common.hpp
main.o: ../../include/fcopss/config.hpp cmd.hpp error.hpp request.hpp
main.o: response.hpp cmd-fibadd.hpp cmd-fibdel.hpp cmd-fibdump.hpp
main.o: cmd-stdel.hpp cmd-stdump.hpp cmd-facedel.hpp cmd-facedump.hpp
cmd-fibadd.o: cmd-fibadd.hpp ../../include/fcopss/common.hpp cmd.hpp
cmd-fibadd.o: error.hpp request.hpp response.hpp
cmd-fibdel.o: cmd-fibdel.hpp ../../include/fcopss/common.hpp cmd.hpp
cmd-fibdel.o: error.hpp request.hpp response.hpp
cmd-fibdump.o: cmd-fibdump.hpp ../../include/fcopss/common.hpp cmd.hpp
cmd-fibdump.o: error.hpp request.hpp response.hpp
cmd-stdel.o: cmd-stdel.hpp ../../include/fcopss/common.hpp cmd.hpp
cmd-stdel.o: error.hpp request.hpp response.hpp
cmd-stdump.o: cmd-stdump.hpp ../../include/fcopss/common.hpp cmd.hpp
cmd-stdump.o: error.hpp request.hpp response.hpp
cmd-facedel.o: cmd-facedel.hpp ../../include/fcopss/common.hpp cmd.hpp
cmd-facedel.o: error.hpp request.hpp response.hpp
cmd-facedump.o: cmd-facedump.hpp ../../include/fcopss/common.hpp cmd.hpp
cmd-facedump.o: error.hpp request.hpp response.hpp
cmd.o: ../../include/fcopss/cd.hpp ../../include/fcopss/common.hpp
cmd.o: ../../include/fcopss/cd-component.hpp
cmd.o: ../../include/fcopss/cd-optional.hpp
cmd.o: ../../include/fcopss/tlv.hpp
cmd.o: ../../include/fcopss/cd-asterisk.hpp cmd.hpp error.hpp request.hpp
cmd.o: response.hpp
request.o: request.hpp ../../include/fcopss/common.hpp
response.o: response.hpp ../../include/fcopss/common.hpp
rtctrl.o: rtctrl.hpp ../../include/fcopss/common.hpp
rtctrl.o: ../../include/fcopss/config.hpp cmd.hpp error.hpp request.hpp
rtctrl.o: response.hpp cmd-fibadd.hpp cmd-fibdel.hpp cmd-fibdump.hpp
rtctrl.o: cmd-stdel.hpp cmd-stdump.hpp cmd-facedel.hpp cmd-facedump.hpp
