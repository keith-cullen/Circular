ID1 = ../Moving/

CC = g++
CFLAGS = -Wall --std=c++17 -I$(ID1)
LD = g++
LDFLAGS = --std=c++17
INCS = Chan.h \
       Chan.hpp \
       $(ID1)/CircBuf.h \
       $(ID1)/CircBuf.hpp
OBJS = testChan.o
LIBS = -lgtest \
       -lpthread
PROG = testChan
RM = /bin/rm -f

$(PROG): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $@ $(LIBS)

%.o: %.cpp $(INCS)
	$(CC) $(CFLAGS) -c $<

clean:
	$(RM) $(PROG) $(OBJS)
