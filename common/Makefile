# Makefile for common library
# Builds and compiles all these utility functions into a library.
#
# Jason Chen
#
# Referenced libcs50 Makefile by David Kotz and Xia Zhou

# object files, and the target library
OBJS = pagedir.o index.o word.o
LIB = common.a

# add -DNOSLEEP to disable the automatic sleep after web-page fetches
#               (students, please do not use -DNOSLEEP!)
# add -DMEMTEST for memory tracking report in indexer
# (and run `make clean; make` whenever you change this)
FLAGS = # -DMEMTEST  # -DNOSLEEP

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../libcs50 $(FLAGS)
CC = gcc
MAKE = make

# Build the library by archiving object files
$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS)

# $(LIB): libcs50-given.a
# 	cp libcs50-given.a $(LIB)

# Dependencies: object files depend on header files
pagedir.o: pagedir.h
index.o: index.h
word.o: word.h

.PHONY: clean sourcelist

# list all the sources and docs in this directory.
# (this rule is used only by the Professor in preparing the starter kit)
sourcelist: Makefile *.md *.c *.h
	@echo $^

# clean up after our compilation
clean:
	rm -f core
	rm -f $(LIB) *~ *.o
