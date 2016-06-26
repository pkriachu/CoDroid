
CC	= gcc
CXX	= g++
JAVA	= javac
CFLAGS	= -Wall -g -I../smaliutil
LDFLAGS	= -L../smaliutil -lsmaliutil #-pthread
DATE	= $(shell date '+%Y%m%d-%H%M')
#DATE	= $(shell date '+%Y%m%d-%H%M%S')

PROGS	= codroid dumpmeta localco Coverage.class TestCoverage.class 

all: $(PROGS)

%.o: %.cpp
	$(CXX) -c $(CFLAGS) $<

codroid: codroid.o
	$(CXX) -o $@ $^ $(LDFLAGS)

dumpmeta: dumpmeta.o
	$(CXX) -o $@ $^ $(LDFLAGS)

localco: localco.o
	$(CXX) -o $@ $^ $(LDFLAGS)

Coverage.class: Coverage.java
	$(JAVA) $<

TestCoverage.class: TestCoverage.java Coverage.class
	$(JAVA) $<

clean:
	rm -f *.o $(PROGS)
