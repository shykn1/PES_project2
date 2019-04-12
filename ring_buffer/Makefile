#Makefile used
INCLUDE_DIRS = 
LIB_DIRS = 
CC=gcc

CDEFS=
CFLAGS= -O0 -g $(INCLUDE_DIRS) $(CDEFS) 
LIBS=

HFILES= 



OFILES_REAGULAR= ring.o ring_test.o
OFILES_RANDOMIZED= ring.o ring_test_random.o
SRCS= ${HFILES} ${CFILES}


	
all:	ring ring_ran

clean:
	-rm -f *.o *.d 
	-rm -f ring

distclean:
	-rm -f *.o *.d

#pes1: pes1.o parser.o
ring: $(OFILES_REAGULAR)
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $^ -lcunit

ring_ran: $(OFILES_RANDOMIZED)
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $^ -lcunit

%.o:%.c
	$(CC) $(CFLAGS) -c $<

.cpp.o:
	$(CC) $(CFLAGS) -c $<


