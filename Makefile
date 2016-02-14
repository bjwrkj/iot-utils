#
# Makefile for the Linux kernel device drivers.
#
# Note! Dependencies are done automagically by 'make dep', which also
# removes any old dependencies. DON'T put your own dependencies here
# unless it's something special (not a .c file).
#
# Note 2! The CFLAGS definitions are now in the main makefile.

#export KSRC := /usr/src/linux

SUBDIRS := $(shell pwd)

ifeq ($(KSRC),)
	KSRC ?= /lib/modules/$(shell uname -r)/build
endif


#DOCS:= ChangeLog COPYING RELEASE_NOTES README README.vmware README.initiators

#ifeq ($(DOCDIR),)
#	DOCDIR := /usr/share/doc/iscsitarget
#endif

all: usrs kernels

usrs: 
	$(MAKE) -C usr

kernels:
	$(MAKE) -C $(KSRC) SUBDIRS=$(shell pwd)/kernel modules
clean:
	$(MAKE) -C usr clean
	$(MAKE) -C $(KSRC) SUBDIRS=$(shell pwd)/kernel clean

