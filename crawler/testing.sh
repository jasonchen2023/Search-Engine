# testing.sh - testing for crawler.c
#
# Jason Chen
# CS50 Lab4

mkdir dir

#ERROR CASES

# invalid input
./crawler

# invalid input (too few arguments)
./crawler http://cs50tse.cs.dartmouth.edu/tse/

#invalid input (too few arguments)
./crawler http://cs50tse.cs.dartmouth.edu/tse/ dir

# invalid pageDirectory
./crawler http://cs50tse.cs.dartmouth.edu/tse/ randomdir 1

# invalid pageDirectory
./crawler http://cs50tse.cs.dartmouth.edu/tse/ dir/dir 1

# invalid maxDepth
./crawler http://cs50tse.cs.dartmouth.edu/tse/ dir -10

# invalid maxDepth
./crawler http://cs50tse.cs.dartmouth.edu/tse/ dir -1

# invalid maxDepth
./crawler http://cs50tse.cs.dartmouth.edu/tse/ dir 11

# invalid maxDepth
./crawler http://cs50tse.cs.dartmouth.edu/tse/ dir six

# invalid server
./crawler http://dartmouthskiingfun.com dir 1

# non-internal server
./crawler https://cs.dartmouth.edu/~cs50/Lectures/crawler/DESIGN.html dir 1

# nonexistent page
./crawler http://cs50tse.cs.dartmouth.edu/tse/index.html dir 1

# too many arguments
./crawler http://cs50tse.cs.dartmouth.edu/tse/ dir 1 hello

rm -rf dir

# ------------- VALID INPUT TESTS ------------------

mkdir dir
# valid input
./crawler http://cs50tse.cs.dartmouth.edu/tse/ dir 0
rm -rf dir
mkdir dir

# valid input
./crawler http://cs50tse.cs.dartmouth.edu/tse/ dir 1
rm -rf dir
mkdir dir

# valid input
./crawler http://cs50tse.cs.dartmouth.edu/tse/ dir 2
rm -rf dir
mkdir dir

# **cross linked webpages**

# depth 0
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html dir 0
rm -rf dir
mkdir dir

# depth 1
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html dir 1
rm -rf dir
mkdir dir

# depth 2
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html dir 2
rm -rf dir
mkdir dir

# depth 3
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html dir 3
rm -rf dir
mkdir dir

# depth 4
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html dir 4
rm -rf dir
mkdir dir

# depth 5
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html dir 5
rm -rf dir
mkdir dir

# changing seedURL
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/B.html dir 5
rm -rf dir
mkdir dir

# testing wikipedia playground
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/ dir 0
rm -rf dir
mkdir dir

# testing wikipedia playground
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/ dir 1
rm -rf dir


exit 0;