INSTDIR=/usr/local
INSTINCDIR=$(INSTDIR)/include
INSTLIBDIR=$(INSTDIR)/lib
INSTBINDIR=$(INSTDIR)/bin

NDNINCS=$(shell pkg-config --cflags-only-I libndn-cxx)
NDNCXXF=$(shell pkg-config pkg-config --cflags-only-other libndn-cxx)
NDNLIBS=$(shell pkg-config --libs libndn-cxx)

HDRDIR=../../include/fcopss

CXX=g++
CXXFLAGS=-std=c++14 -Wall $(NDNCXXF) -O2 -g -fPIC
INCFLAGS=-I. -I../../include
CPPFLAGS=$(INCFLAGS) $(NDNINCS)
LDFLAGS=-L/usr/lib/x86_64-linux-gnu
LDLIBS=$(NDNLIBS)

AR=ar
RANLIB=ranlib

SRCS+=cd-optional.cpp
SRCS+=cd-asterisk.cpp
SRCS+=cd.cpp
SRCS+=sub.cpp
SRCS+=pub.cpp
SRCS+=pub-to-rp.cpp
SRCS+=pub-from-rp.cpp
SRCS+=tcp-transport.cpp
SRCS+=udp-transport.cpp
SRCS+=face.cpp
SRCS+=config.cpp
SRCS+=log-private.cpp

OBJS=$(SRCS:%.cpp=%.o)

MAJOR = 1
MINOR = 0
PATCH = 0
LIBNAME = libfcopss
LDLIBNAME = $(LIBNAME).so
SOLIBNAME = $(LDLIBNAME).$(MAJOR)
LDSOFNAME = $(SOLIBNAME).$(MINOR).$(PATCH)
ARLIBNAME = $(LIBNAME).a

all: $(LDSOFNAME) $(ARLIBNAME)

$(LDSOFNAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -shared -o $(LDSOFNAME) -Wl,-soname -Wl,$(SOLIBNAME) $(OBJS)
	$(RM) $(SOLIBNAME) && ln -sf $(LDSOFNAME) $(SOLIBNAME)
	$(RM) $(LDLIBNAME) && ln -sf $(SOLIBNAME) $(LDLIBNAME)

$(ARLIBNAME): $(OBJS)
	$(AR) -r $(ARLIBNAME) $(OBJS) >/dev/null 2>&1
	$(RANLIB) $(ARLIBNAME)

install: all
	mkdir -p $(INSTINCDIR)/fcopss
	rm -f $(INSTLIBDIR)/$(SOLIBNAME)*
	cp -p $(LDSOFNAME) $(INSTLIBDIR)
	ln -sf $(LDSOFNAME) $(INSTLIBDIR)/$(SOLIBNAME)
	rm -f $(INSTLIBDIR)/$(LDLIBNAME)
	ln -sf $(SOLIBNAME) $(INSTLIBDIR)/$(LDLIBNAME)
	rm -f $(INSTLIBDIR)/$(ARLIBNAME)
	cp -p $(ARLIBNAME) $(INSTLIBDIR)
	for hdr in $(HDRDIR)/*.hpp ; do cp -p $$hdr $(INSTINCDIR)/fcopss ; done

uninstall:
	rm -rf $(INSTINCDIR)/fcopss
	rm -f $(INSTLIBDIR)/$(LIBNAME).*

clean:
	rm -rf $(PROG) ut.o $(OBJS) $(LDLIBNAME)* $(ARLIBNAME)

PROG=ut

$(PROG) : ut.o $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ ut.o $(OBJS) $(LDFLAGS) $(LDLIBS)

depend:
	makedepend -Y -- $(INCFLAGS) -- $(SRCS) >/dev/null 2>&1
	rm -f Makefile.bak

# DO NOT DELETE

cd-optional.o: ../../include/fcopss/cd-optional.hpp
cd-optional.o: ../../include/fcopss/common.hpp ../../include/fcopss/tlv.hpp
cd-asterisk.o: ../../include/fcopss/cd-asterisk.hpp
cd-asterisk.o: ../../include/fcopss/common.hpp ../../include/fcopss/tlv.hpp
cd.o: ../../include/fcopss/cd.hpp ../../include/fcopss/common.hpp
cd.o: ../../include/fcopss/cd-component.hpp
cd.o: ../../include/fcopss/cd-optional.hpp ../../include/fcopss/tlv.hpp
cd.o: ../../include/fcopss/cd-asterisk.hpp
sub.o: ../../include/fcopss/sub.hpp ../../include/fcopss/common.hpp
sub.o: ../../include/fcopss/cd.hpp ../../include/fcopss/cd-component.hpp
sub.o: ../../include/fcopss/cd-optional.hpp ../../include/fcopss/tlv.hpp
sub.o: ../../include/fcopss/cd-asterisk.hpp
pub.o: ../../include/fcopss/pub.hpp ../../include/fcopss/common.hpp
pub.o: ../../include/fcopss/cd.hpp ../../include/fcopss/cd-component.hpp
pub.o: ../../include/fcopss/cd-optional.hpp ../../include/fcopss/tlv.hpp
pub.o: ../../include/fcopss/cd-asterisk.hpp
pub-to-rp.o: ../../include/fcopss/pub-to-rp.hpp
pub-to-rp.o: ../../include/fcopss/common.hpp ../../include/fcopss/pub.hpp
pub-to-rp.o: ../../include/fcopss/cd.hpp
pub-to-rp.o: ../../include/fcopss/cd-component.hpp
pub-to-rp.o: ../../include/fcopss/cd-optional.hpp
pub-to-rp.o: ../../include/fcopss/tlv.hpp
pub-to-rp.o: ../../include/fcopss/cd-asterisk.hpp
pub-from-rp.o: ../../include/fcopss/pub-from-rp.hpp
pub-from-rp.o: ../../include/fcopss/common.hpp ../../include/fcopss/pub.hpp
pub-from-rp.o: ../../include/fcopss/cd.hpp
pub-from-rp.o: ../../include/fcopss/cd-component.hpp
pub-from-rp.o: ../../include/fcopss/cd-optional.hpp
pub-from-rp.o: ../../include/fcopss/tlv.hpp
pub-from-rp.o: ../../include/fcopss/cd-asterisk.hpp
pub-from-rp.o: ../../include/fcopss/pub-to-rp.hpp
tcp-transport.o: ../../include/fcopss/tcp-transport.hpp
tcp-transport.o: ../../include/fcopss/transport.hpp
tcp-transport.o: ../../include/fcopss/common.hpp ../../include/fcopss/sub.hpp
tcp-transport.o: ../../include/fcopss/cd.hpp
tcp-transport.o: ../../include/fcopss/cd-component.hpp
tcp-transport.o: ../../include/fcopss/cd-optional.hpp
tcp-transport.o: ../../include/fcopss/tlv.hpp
tcp-transport.o: ../../include/fcopss/cd-asterisk.hpp
tcp-transport.o: ../../include/fcopss/pub-from-rp.hpp
tcp-transport.o: ../../include/fcopss/pub.hpp
tcp-transport.o: ../../include/fcopss/pub-to-rp.hpp
tcp-transport.o: ../../include/fcopss/config.hpp ../../include/fcopss/log.hpp
tcp-transport.o: ../../include/fcopss/log-private.hpp
udp-transport.o: ../../include/fcopss/udp-transport.hpp
udp-transport.o: ../../include/fcopss/transport.hpp
udp-transport.o: ../../include/fcopss/common.hpp ../../include/fcopss/sub.hpp
udp-transport.o: ../../include/fcopss/cd.hpp
udp-transport.o: ../../include/fcopss/cd-component.hpp
udp-transport.o: ../../include/fcopss/cd-optional.hpp
udp-transport.o: ../../include/fcopss/tlv.hpp
udp-transport.o: ../../include/fcopss/cd-asterisk.hpp
udp-transport.o: ../../include/fcopss/pub-from-rp.hpp
udp-transport.o: ../../include/fcopss/pub.hpp
udp-transport.o: ../../include/fcopss/pub-to-rp.hpp
udp-transport.o: ../../include/fcopss/config.hpp ../../include/fcopss/log.hpp
udp-transport.o: ../../include/fcopss/log-private.hpp
face.o: ../../include/fcopss/face.hpp ../../include/fcopss/common.hpp
face.o: ../../include/fcopss/sub.hpp ../../include/fcopss/cd.hpp
face.o: ../../include/fcopss/cd-component.hpp
face.o: ../../include/fcopss/cd-optional.hpp ../../include/fcopss/tlv.hpp
face.o: ../../include/fcopss/cd-asterisk.hpp
face.o: ../../include/fcopss/pub-from-rp.hpp ../../include/fcopss/pub.hpp
face.o: ../../include/fcopss/pub-to-rp.hpp ../../include/fcopss/transport.hpp
face.o: ../../include/fcopss/tcp-transport.hpp
face.o: ../../include/fcopss/udp-transport.hpp
face.o: ../../include/fcopss/config.hpp ../../include/fcopss/log.hpp
face.o: ../../include/fcopss/log-private.hpp
config.o: ../../include/fcopss/common.hpp ../../include/fcopss/config.hpp
log-private.o: ../../include/fcopss/log-private.hpp
log-private.o: ../../include/fcopss/common.hpp
