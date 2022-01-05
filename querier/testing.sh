# Testing for querier.c

# Jason Chen
# CS 50, Lab 6

# run crawler and indexer to create necessary files
mkdir tse1/
../crawler/crawler http://cs50tse.cs.dartmouth.edu/tse/ tse1 1
../indexer/indexer tse1 tseindex1

# run crawler and indexer to create necessary files
mkdir tse2/
../crawler/crawler http://cs50tse.cs.dartmouth.edu/tse/ tse2 2
../indexer/indexer tse2 tseindex2

# valid parameters
./querier tse2 tseindex2

#valid parameters
./querier tse1 tseindex1

# fuzzquery
./fuzzquery tseindex2 5 0 | ./querier tse2 tseindex2

# fuzzquery
./fuzzquery tseindex2 10 3 | ./querier tse2 tseindex2

# fuzzquery
./fuzzquery tseindex2 15 2 | ./querier tse1 tseindex1

# fuzzquery
./fuzzquery tseindex2 10 1 | ./querier tse1 tseindex1

# error cases

# invalid pageDirectory
./querier dartmouth tseindex2

# invalid pageDirectory
./querier dartmo////uth tseindex2

# invalid pageDirectory
./querier 3 tseindex2

# invalid indexfile
./querier tse2 dartmouth

# invalid indexfile
./querier tse2 dartmouth/

# invalid indexfile
./querier tse2 4

# invalid pageDirectory and indexfile
./querier dartmouth dartmouth

# invalid input
./querier

# invalid input
./querier tse2

# invalid input
./querier tse2 tseindex2 dartmouth


rm -rf tse*


exit 0