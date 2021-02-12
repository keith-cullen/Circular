CC = gcc
CFLAGS = -Wall -g
LD = gcc
LDFLAGS =
INCS = circ_buf.h
OBJS = test_circ_buf.o circ_buf.o
LIBS =
PROG = test_circ_buf
MACROS = test_macros
RM = /bin/rm -f

$(PROG): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $(PROG) $(LIBS)

test_circ_buf.o: test_circ_buf.c $(INCS)
	$(CC) $(CFLAGS) -c test_circ_buf.c

circ_buf.o: circ_buf.c $(INCS)
	$(CC) $(CFLAGS) -c circ_buf.c

clean:
	$(RM) $(PROG) $(OBJS) $(MACROS)
