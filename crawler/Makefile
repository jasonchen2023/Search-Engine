# Makefile for crawler.c
#
# Jason Chen
# CS50, February 8, 2021

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb
PROG = crawler
OBJS = crawler.o ../common/common.a
LIBS = ../libcs50/libcs50-given.a

.PHONY: all clean

#all: crawler

# executable depends on object files
$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(PROG)

# object files depend on include files
crawler.o: ../libcs50/webpage.h ../libcs50/hashtable.h ../libcs50/bag.h 

test: testing.sh
	bash -v testing.sh

clean:
	rm -f $(PROG)
	rm -f *~ *.o
	rm -rf *.dSYM