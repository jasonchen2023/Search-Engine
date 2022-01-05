# Makefile for querier.c
#
# Jason Chen
# CS50, February 24, 2021

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb
PROG = querier
OBJS = querier.o
# LIBS = ../common/common.a ../libcs50/libcs50-given.a
LIBS = ../common/common.a ../libcs50/libcs50.a

.PHONY: all clean


# # executable depends on object files
$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(PROG)


test: testing.sh
	bash -v testing.sh

clean:
	rm -f $(PROG)
	rm -f *~ *.o
	rm -rf *.dSYM