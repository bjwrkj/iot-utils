CFLAGS += -O2 -fno-inline -Wall -Wstrict-prototypes -g -I../include
CFLAGS += -D_GNU_SOURCE # required for glibc >= 2.8
PROGRAMS = iot 
LIBS =

all: $(PROGRAMS)

iot: iot.o 

	$(CC) $^ -o $@ $(LIBS)

clean:
	rm -f *.o $(PROGRAMS)