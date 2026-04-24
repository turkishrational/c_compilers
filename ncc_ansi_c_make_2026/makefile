CC=gcc
CFLAGS=-g

all::

#
# cc1 - compiler proper
#

CC1_HDRS=cc1/cc1.h cc1/string.h cc1/output.h cc1/error.h cc1/lex.h cc1/token.h

CC1_OBJS=cc1/cc1.o cc1/string.o cc1/output.o cc1/lex.o cc1/error.o cc1/util.o

cc1/cc1.o: cc1/cc1.c $(CC1_HDRS)
cc1/string.o: cc1/string.c cc1/cc1.h cc1/string.h cc1/util.h
cc1/output.o: cc1/output.c cc1/cc1.h cc1/output.h cc1/error.h
cc1/lex.o: cc1/lex.c cc1/cc1.h cc1/string.h cc1/output.h cc1/token.h cc1/error.h cc1/lex.h
cc1/error.o: cc1/error.c cc1/cc1.h cc1/string.h cc1/output.h cc1/error.h
cc1/util.o: cc1/util.c cc1/util.h cc1/error.h

cc1/cc1: $(CC1_OBJS)
	$(CC) $(CFLAGS) -o cc1/cc1 $(CC1_OBJS)

all:: cc1/cc1

clean::
	rm -f cc1/cc1 cc1/*.o
