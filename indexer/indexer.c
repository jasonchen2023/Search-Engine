/*
indexer.c 

Jason Chen
CS50, Winter 2021

The TSE indexer is a standalone program that reads the document files produced by the TSE crawler, builds an index, and writes that index to a file.

Usage: ./indexer pageDirectory indexFilename

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
#define _GNU_SOURCE


hashtable_t *ht;
const int hashtable_slots = 200;


void itemdelete(void *item);

int main(int argc, char* argv[]) {

    if (argc != 3) {
        printf("Usage: ./indexer pageDirectory indexFilename \n");
        return 1;
    }

    // check if the second argument is a valid directory
    char *pageDirectory = argv[1];
    if (!pagedirExists(pageDirectory)) {
        printf("directory does not exist \n");
        return 1;
    }


    char *indexfile = argv[2];
    FILE *newfp = fopen(indexfile, "w");

    // if the file was not opened successfully
    if (newfp == 0) {
        printf("index file unsucessfully opened\n");
        return 1;
    } else {
        fclose(newfp);
    }


    //build the data structure: hashtable of counters

    ht = hashtable_new(hashtable_slots);
    if (ht == NULL) {
        printf("Error creating hashtable \n");
        return 1;
    }
    
    // open the first file in pageDirectory (ID 1)
    int ID = 1;
    char *pagedir = malloc(strlen(pageDirectory) + 1);
    strcpy(pagedir, pageDirectory);
    char *file = malloc(strlen(pagedir) + strlen("/1000") + 2);
    sprintf(file, "%s/%d", pagedir, ID);

    FILE *fpointer = fopen (file, "r");   


    // loop through every file in pageDirectory and call indexCreate
    while (fpointer != NULL) {

        webpage_t *page = loadpage(fpointer);   // create a new webpage according to the file

        indexCreate(page, ht, ID);

        fclose(fpointer);
        webpage_delete(page);

        // update filename with new ID
        ID++;
        sprintf(file, "%s/%d", pagedir, ID);
        fpointer = fopen (file, "r");    
    }


    free(file);
    free(pagedir);


    indexSave(ht, indexfile);   // save index to the file

    hashtable_delete(ht, *itemdelete);

    return 0;

}




// helper function to delete counters
void itemdelete(void *item) {
    counters_t *ctrs = item;
    counters_delete(ctrs);
}
