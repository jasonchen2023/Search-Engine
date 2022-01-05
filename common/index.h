/*
 * index.h - header file for index.c
 * 
 * Jason Chen
 * CS50, Lab 5
 */



#ifndef __INDEX_H
#define __INDEX_H
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include "../libcs50/webpage.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"

/**************** indexCreate ****************/
/* 
 * Used by indexer.c
 * 
 * Creates an inverted index data structure with words as key, and counters of (ID, count) pairs as item
 * Caller provides:
 *   a webpage, hashtable, and ID of file
 *  
 */
void indexCreate(webpage_t *page, hashtable_t *ht, int ID);

/**************** indexLoad ****************/
/* 
 * Used by indextest.c
 * 
 * Creates an inverted index data structure with words as key, and counters of (ID, count) pairs as item
 * Caller provides:
 *   hashtable and name of index file created by indexer.c
 *  
 */
void indexLoad(hashtable_t *ht, char *oldIndexFile);

/**************** indexSave ****************/
/* 
 * Writes the inverted index data structure to a file
 * Caller provides:
 *   a hashtable and string filename
 * 
 * Note:
 *  each line provides the word and one or more (docID, count) pairs, in the format word docID count [docID count]...
 *  
 */
void indexSave(hashtable_t *ht, char *filename);



#endif // __INDEX_H