CC = g++
CFLAGS = -Wall --std=c++17
LD = g++
LDFLAGS = --std=c++17
INCS = CircBuf.h \
       CircBuf.hpp
OBJS = testCircBuf.o
LIBS = -lgtest \
       -lpthread
PROG = testCircBuf
RM = /bin/rm -f

$(PROG): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $@ $(LIBS)

%.o: %.cpp $(INCS)
	$(CC) $(CFLAGS) -c $<

clean:
	$(RM) $(PROG) $(OBJS)
