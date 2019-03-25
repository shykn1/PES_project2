#Makefile used
INCLUDE_DIRS = 
LIB_DIRS = 
CC=gcc

CDEFS=
CFLAGS= -O0 -g $(INCLUDE_DIRS) $(CDEFS) 
LIBS=

HFILES= 



OFILES= ring.o ring_test.o
SRCS= ${HFILES} ${CFILES}


	
all:	ring

clean:
	-rm -f *.o *.d 
	-rm -f ring

distclean:
	-rm -f *.o *.d

#pes1: pes1.o parser.o
ring: $(OFILES)
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $^ -lcunit


%.o:%.c
	$(CC) $(CFLAGS) -c $<

.cpp.o:
	$(CC) $(CFLAGS) -c $<


