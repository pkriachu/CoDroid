
CC	= gcc
CXX	= g++
CFLAGS	= -Wall

OBJS	= smalitype.o smaliparse.o smalicfg.o smaliinst.o smaliio.o

all: libsmaliutil.a

%.o: %.cpp
	$(CXX) -c $(CFLAGS) $<

libsmaliutil.a: $(OBJS)
	ar rc $@ $^

clean:
	rm -f libsmaliutil.a *.o

