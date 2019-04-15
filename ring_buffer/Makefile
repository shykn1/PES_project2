#Makefile used
INCLUDE_DIRS = 
LIB_DIRS = 
CC=gcc

CDEFS=
CFLAGS= -O0 -g $(INCLUDE_DIRS) $(CDEFS) 
LIBS=

HFILES= 



OFILES_REAGULAR= ./src/ring.o ./Unittest/ring_test.o
OFILES_RANDOMIZED= ./src/ring.o ./Unittest/ring_test_random.o

SRCS= ${HFILES} ${CFILES}


	
all:	ring ring_ran

clean:
	-rm -f *.o *.d 
	-rm -f ring
	-rm -f ring_ran

distclean:
	-rm -f *.o *.d

#pes1: pes1.o parser.o
ring: $(OFILES_REAGULAR)
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $^ -lcunit

ring_ran: $(OFILES_RANDOMIZED)
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $^ -lcunit

src\%.o:src\%.c
	$(CC) $(CFLAGS)     -c   $<

Unittest\%.o:Unittest\%.c
	$(CC) $(CFLAGS)     -c   $<




