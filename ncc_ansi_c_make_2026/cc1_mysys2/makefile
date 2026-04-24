CC=gcc
CFLAGS=-g -std=c17

all::

#
# cc1 - compiler proper
#

CC1_HDRS=cc1.h string.h output.h error.h lex.h token.h

CC1_OBJS=cc1.o string.o output.o lex.o error.o util.o

cc1.o: cc1.c $(CC1_HDRS)
string.o: string.c cc1.h string.h util.h
output.o: output.c cc1.h output.h error.h
lex.o: lex.c cc1.h string.h output.h token.h error.h lex.h
error.o: error.c cc1.h string.h output.h error.h
util.o: util.c util.h error.h

cc1: $(CC1_OBJS)
	$(CC) $(CFLAGS) -o cc1 $(CC1_OBJS)

all:: cc1

clean::
	rm -f cc1 *.o
