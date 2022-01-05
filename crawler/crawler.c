/*
crawler.c 

Jason Chen
CS50, Winter 2021

The crawler crawls a website and retrieves webpages starting with a specified URL. 
It parses the initial webpage, extracts any embedded URLs and retrieves those pages, and crawls the pages found at those URLs.
It limits itself to maxDepth hops from the seed URL and to URLs that are ‘internal’ to the designated CS50 server.

Usage: ./crawler URL pageDirectory maxDepth

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include "../libcs50/webpage.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/bag.h"
#include "../common/pagedir.h"
#define _GNU_SOURCE


char *URL;
char *directory;
int ID;
int maxdepth;
char *lastchar;
hashtable_t *ht;
bag_t *bag;
const int hashtable_slots = 150;

void crawler(char *URL, hashtable_t *ht, bag_t *bag);
inline static void logr(const char *word, const int depth, const char *url);


int main(int argc, char* argv[]) {

    if (argc != 4) {
        printf("Usage: ./crawler seedURL pageDirectory maxDepth \n");
        return 1;
    }

    // check if URL is valid and internal
    URL = malloc(strlen(argv[1]) + 1);
    strcpy(URL, argv[1]);
    if (IsInternalURL(URL) == 0) {
        printf("Invalid or non-internal URL \n");
        return 1;
    }

    // check if the second argument is a valid directory
    directory = argv[2];
    if (!pagedirExists(directory)) {
        printf("directory does not exist \n");
        free(URL);
        return 1;
    }

    //check if depth in range
    maxdepth = strtold(argv[3], &lastchar);
    if (*lastchar != '\0' || maxdepth < 0 || maxdepth > 10) {
        printf("depth must be between 0 and 10 \n");
        return 1;
    }

    // create a new hashtable to store visited URLs
    ht = hashtable_new(hashtable_slots);
    if (ht == NULL) {
        printf("Error creating hashtable \n");
        return 1;
    }

    // create a bag to store unvisited websites
    bag = bag_new();
    if (bag == NULL) {
        printf("Error creating bag \n");
        return 1;
    }

    // loop over pages to explore, until the list is exhausted
    crawler(URL, ht, bag);

    bag_delete(bag, webpage_delete);
    hashtable_delete(ht, NULL);

    return 0;

}


// loop over pages to explore starting at seed URL, until the list is exhausted
void crawler(char *URL, hashtable_t *ht, bag_t *bag) {
        // create webpage for rootURL, insert to bag and hashtable
    webpage_t *webpage = webpage_new(URL, 0, NULL);
    bag_insert(bag, webpage);
    hashtable_insert(ht, URL, "");
    
    
    while ((webpage = bag_extract(bag)) != NULL) {
        logr("Fetched", webpage_getDepth(webpage), webpage_getURL(webpage));

        ID++;
        if (pagefetcher(webpage)) {   // fetch the html of this webpage

            pagesaver(webpage, directory, ID); // write the contents of the page to a file
            // printf("pagesaved \n");
        // if the webpage's depth is less than max depth, scan for valid links
            if (webpage_getDepth(webpage) < maxdepth) {
                logr("Scanning", webpage_getDepth(webpage), webpage_getURL(webpage));
                pagescanner(webpage, ht, bag);
                // printf("pagescanned\n");
            }
        } else {
            logr("Fetch failed", webpage_getDepth(webpage), webpage_getURL(webpage));
        }
        webpage_delete(webpage);
        // printf("Deleted \n");
    }
}


// log one word (1-9 chars) about a given url                                   
inline static void logr(const char *word, const int depth, const char *url)
{
  printf("%2d %*s%9s: %s\n", depth, depth, "", word, url);
}

