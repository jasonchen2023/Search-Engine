/*
indextest.c
 
Jason Chen
CS50, Winter 2021
 
the index tester, loads an index file produced by the indexer and saves it to another file.
 
Usage: ./indextest oldIndexFile newIndexFile
 
*/
 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>
#include "../libcs50/webpage.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"
#include "../common/pagedir.h"
#include "../common/index.h"
#include "../libcs50/file.h"
#define _GNU_SOURCE


hashtable_t *ht;
const int hashtable_slots = 150;
 
 
void itemdelete(void *item);
 
int main(int argc, char* argv[]) {
 
   if (argc != 3) {
       printf("Usage: ./indexer oldIndexName newIndexFile \n");
       return 1;
   }
 
   // check if first argument is a readable file
   char *oldIndexFile = argv[1];
   FILE *oldfp = fopen(oldIndexFile, "r");
 
   // if the file was not opened successfully
   if (oldfp == 0) {
       printf("oldIndexFile unsucessfully opened or read\n");
       return 1;
   } else {
       fclose(oldfp);
   }
 
   // chcek if newIndexFile is writable
   char *newIndexFile = argv[2];
   FILE *newfp = fopen(newIndexFile, "w");
 
   // if the file was not opened successfully
   if (newfp == 0) {
       printf("newIndexFile unsucessfully opened\n");
       return 1;
   } else {
       fclose(newfp);
   }


    // create the hashtable
   ht = hashtable_new(hashtable_slots);
   if (ht == NULL) {
       printf("Error creating hashtable \n");
       return 1;
   }
 
    indexLoad(ht, oldIndexFile);    // create the inverted index structure based on oldIndexFile
 
    indexSave(ht, newIndexFile);    // write the index to a new file


    hashtable_delete(ht, *itemdelete);

    return 0;

}


// helper function to delete counters
void itemdelete(void *item) {
    counters_t *ctrs = item;
    counters_delete(ctrs);
}

 
 

