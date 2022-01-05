
# Jason Chen
## CS50 Lab 6
## February 24, 2021

The TSE Querier is a standalone program that reads the index file produced by the TSE Crawler, and page files produced by the TSE Querier, and answers search queries submitted via stdin

Usage: 
• `./querier pageDirectory indexFilename`

Note: My querier meets the full specs and prints the document set in decreasing order by score.

### Assumptions

• `dirnameExists` located within pagedir.c in 'common' directory
• `indexLoad` located within index.c in 'common' directory
• `normalizeWord` located within words.c in 'common' directory

• number of slots for hashtable = 200


### Compilation
• to compile, simply `make`
