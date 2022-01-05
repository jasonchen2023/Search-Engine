/*
 * index.c - functions that save and load index files
 * 
 * Jason Chen
 * CS 50, Lab5
 * 
 */



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include "../libcs50/webpage.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"
#include "../libcs50/file.h"
#include "word.h"

#define _GNU_SOURCE

static void wordprint(FILE *fp, const char *key, void *item);

// add words to the inverted index structure based on webpage
void indexCreate(webpage_t *page, hashtable_t *ht, int ID) {
    int pos = 0;
    char *result;

    // loop through all words in webpage
    while ((result = webpage_getNextWord(page, &pos)) != NULL) {
        if (strlen(result) >= 3) {
            if (result != NULL) {
                normalizeWord(result);
            } else {
                continue;
            }

            counters_t *ctrs = hashtable_find(ht, result);

            // if the word is not in hashtable, create a new counters set and insert into hashtable
            if (ctrs == NULL) {
                ctrs = counters_new();
                hashtable_insert(ht, result, ctrs);

            }
            counters_add(ctrs, ID); // increments count by one or creates a new (ID, count) pair if not already exists

        }
        free(result);
    }
}

// Loads an index according to oldIndexFile, which was created by indexer.c
void indexLoad(hashtable_t *ht, char *oldIndexFile) {
   // read the words of the old index file
   FILE *oldfp = fopen(oldIndexFile, "r");
   char *word;
   int docID;
   int count;
   // read every word and (ID, count) pair in file
   while ((word = freadwordp(oldfp)) != NULL) {
       
       while((fscanf(oldfp, "%d %d", &docID, &count)) == 2) {

           if (word != NULL && docID > 0) {
            
                counters_t *ctrs = hashtable_find(ht, word);

                // if the word is not in hashtable
                if (ctrs == NULL) {
                    ctrs = counters_new();
                    hashtable_insert(ht, word, ctrs);
                }

                counters_set(ctrs, docID, count);   // create (ID, count) pair
           }
       }
       free(word);
   }

    fclose(oldfp);
}

// writes the inverted-index to a file
void indexSave(hashtable_t *ht, char *filename) {
    FILE *fptr;
    fptr = fopen(filename, "w");
    hashtable_print(ht, fptr, wordprint);
    fclose(fptr);
}

// helper method to print counters
static void wordprint(FILE *fp, const char *key, void *item)
{
    counters_t *ctrs = item;
    if (ctrs == NULL) {
        fprintf(fp, "(null");
    } else {
        fprintf(fp, "%s ", key);
        counters_print(ctrs, fp);
    }
}