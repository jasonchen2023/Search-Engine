# Makefile for indexer.c
#
# Jason Chen
# CS50, February 8, 2021

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb
PROG = indexer
OBJS = indexer.o
# LIBS = ../common/common.a ../libcs50/libcs50-given.a
LIBS = ../common/common.a ../libcs50/libcs50.a

.PHONY: all clean

all: indexer indextest

indexer: indexer.c indexer.o
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(PROG)

indextest: indextest.c indextest.o
	$(CC) $(CFLAGS) indextest.o $(LIBS) -o indextest


# # executable depends on object files
# $(PROG): $(OBJS)
# 	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(PROG)

# object files depend on include files
indexer.o: ../libcs50/webpage.h ../libcs50/hashtable.h

indextest.o: ../libcs50/hashtable.h

test: testing.sh
	bash -v testing.sh

clean:
	rm -f $(PROG)
	rm -f *~ *.o
	rm -rf *.dSYM