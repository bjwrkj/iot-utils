#
# Makefile for the Linux kernel device drivers.
#
# Note! Dependencies are done automagically by 'make dep', which also
# removes any old dependencies. DON'T put your own dependencies here
# unless it's something special (not a .c file).
#
# Note 2! The CFLAGS definitions are now in the main makefile.
EXTRA_CFLAGS += -I$(src)/../include 

obj-m		+= iot.o
iot-objs	:= iot_core.o fs2416/fs2416.o s3c2410/s3c2410.o\
	
