export INSTDIR=/usr/local
export INSTINCDIR=$(INSTDIR)/include
export INSTLIBDIR=$(INSTDIR)/lib
export INSTBINDIR=$(INSTDIR)/bin

SUBDIR+=lib
SUBDIR+=publisher
SUBDIR+=subscriber
SUBDIR+=router
SUBDIR+=rtctrl
SUBDIR+=rendezvous

all:
	@ for dir in $(SUBDIR) ; do $(MAKE) -e -C $$dir ; ST=$$?; if [ $$ST != 0 ]; then exit $$ST ; fi done

clean:
	@ for dir in $(SUBDIR) ; do $(MAKE) -e -C $$dir $@; ST=$$?; if [ $$ST != 0 ]; then exit $$ST ; fi done

install:
	@ for dir in $(SUBDIR) ; do $(MAKE) -e -C $$dir $@; ST=$$?; if [ $$ST != 0 ]; then exit $$ST ; fi done

uninstall:
	@ for dir in $(SUBDIR) ; do $(MAKE) -e -C $$dir $@; ST=$$?; if [ $$ST != 0 ]; then exit $$ST ; fi done
