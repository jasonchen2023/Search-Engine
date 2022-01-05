# testing.sh - testing for indexer and indextest

# Jason Chen
# CS 50, Lab 5

# call crawler to create the directory
mkdir ../crawler/dir
../crawler/crawler http://cs50tse.cs.dartmouth.edu/tse/ ../crawler/dir 1

# valid inputs
./indexer ../crawler/dir indexfile

# testing indextest
./indextest indexfile newindexfile

# sort and compare
gawk -f indexsort.awk indexfile > indexfile.sorted
gawk -f indexsort.awk newindexfile > newindexfile.sorted 
diff indexfile.sorted newindexfile.sorted

# testing empty directory
mkdir emptydir
./indexer emptydir emptyfile
./indextest emptydir newemptyfile

#sort and compare
gawk -f indexsort.awk emptyfile > emptyfile.sorted
gawk -f indexsort.awk newemptyfile > newemptyfile.sorted 
diff emptyfile newemptyfile


# invalid inputs
./indexer ../crawler/dir indexfile/

# nonexistent file
./indexer ../crawler/nodir file

# nonexistent file
./indexer nossdir file

# invalid inputs
./indexer ../crawler/dir

# invalid inputs
./indexer ../crawler/dir file anotherfile

# -------indextest-------
# nonexistent file
./indextest randomfile newindexfile

# invalid filename
./indextest indexfile/ newindexfile

# invalid filename
./indextest indexfile newindexfile/

# invalid input
./indextest indexfile

# invalid filename
./indextest indexfile/ newindexfile anotherfile

# remove the files
rm indexfile*
rm newindexfile*
rm emptyfile*
rm newemptyfile*
rm -rf emptydir
rm -rf ../crawler/dir




exit 0
