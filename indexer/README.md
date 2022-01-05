
# Jason Chen
## CS50 Lab 5
## February 17, 2021

The TSE indexer is a standalone program that reads the document files produced by the TSE crawler, builds an index, and writes that index to a file. Its companion, the index tester, loads an index file produced by the indexer and saves it to another file.

Usage: 
• `./indexer pageDirectory indexFileName`
• `./indextest oldIndexName newIndexName`

### Assumptions

• `dirnameExists` located within pagedir.c in 'common' directory
• `indexCreate` located within index.c in 'common' directory
• `indexLoad` located within index.c in 'common' directory
• `indexSave` located within index.c in 'common' directory
• `normalizeWord` located within words.c in 'common' directory

• number of slots for hashtable = 200

• file written in format:
    word ID count ID count ...


### Compilation
• to compile, simply `make`
