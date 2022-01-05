/*

pagedir.c - functions for initializing the “pageDirectory” and saving webpages there

Jason Chen
CS 50 Lab4
February 9, 2021

*/

#include <string.h> 
#include <stdbool.h>
#include "../libcs50/webpage.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/bag.h"
#include "../libcs50/file.h"
#define _GNU_SOURCE

inline static void logr(const char *word, const int depth, const char *url);


// Returns whether the pathname provided as pageDirectory is indeed a directory
bool pagedirExists(char *pageDirectory) {
    // allocate memory for a string for filename: "(dir)/.crawlers"
    char *dir = malloc(strlen(pageDirectory) + 1);
    strcpy(dir, pageDirectory);
    char *filename = malloc(strlen(dir) + strlen("/.crawlers") + 2);
    strcpy(filename, dir);
    strcat(filename, "/.crawlers");

    FILE *fp = fopen(filename, "w");

    // if the file was not opened successfully, return false
    if (fp == 0) {

        free(dir);
        free(filename);
        return false;

    } else {

        free(dir);
        fclose(fp);
        free(filename);
        return true;
    }
 
}


// which fetches a page from a URL
bool pagefetcher(webpage_t *webpage) {
    return webpage_fetch(webpage);
}



// create a file for each webpage
void pagesaver(webpage_t *webpage, char *directory, int ID) {

    FILE *fp;

    // allocate memory for filename
    char *dir = malloc(strlen(directory) + 1);
    strcpy(dir, directory);
    char *file = malloc(strlen(directory) + strlen("/1000") + 2);
    sprintf(file, "%s/%d", dir, ID);

    fp = fopen (file, "w");
    fprintf(fp, "%s\n", webpage_getURL(webpage));   // first line: URL
    fprintf(fp, "%d\n", webpage_getDepth(webpage)); // second line: depth

    // add the page contents
    fprintf(fp, webpage_getHTML(webpage));
    fclose(fp);

    free(dir);
    free(file);
}



// parse the webpage to extract all its embedded URLs
void pagescanner(webpage_t *webpage, hashtable_t *ht, bag_t *bag) {
    int pos = 0;
    char *result;
    
    // while there are more URLs in the webpage
    while ((result = webpage_getNextURL(webpage, &pos)) != NULL) {
        //printf("Found url: %s\n", result);
        logr("Found", webpage_getDepth(webpage) + 1, result);

        // if URL is internal, add new webpage to hashtable and bag
        NormalizeURL(result); 
        if (IsInternalURL(result) == 1 ){

            if (hashtable_insert(ht, result, "") == 1) {
                webpage_t *page = webpage_new(result, webpage_getDepth(webpage) + 1, NULL);
                // printf("Webpage created with URL %s: \n", webpage_getURL(page));
                bag_insert(bag, page);
                //printf("Added: %s\n", result);
                logr("added", webpage_getDepth(webpage) + 1, result);

            } else {
                // printf("Ignored Duplicate: %s\n", result);
                logr("IgnDupl", webpage_getDepth(webpage) + 1, result);
                free(result);
            }

        }else {
            //printf("Ignored External: %s\n", result);
            logr("IgnExtrn", webpage_getDepth(webpage) + 1, result);
            free(result);
        }
    }

}

// log one word (1-9 chars) about a given url                                   
inline static void logr(const char *word, const int depth, const char *url)
{
  printf("%2d %*s%9s: %s\n", depth, depth, "", word, url);
}

// creates a new webpage based on the crawler file
webpage_t *loadpage(FILE *file) {

    // save the url, depth, and html to variables
    char *URL = freadlinep(file);
    char *depth = freadlinep(file);
    char *HTML = freadfilep(file);

    int dep = atoi(depth);
    free(depth);

    // create and return new webpage
    webpage_t *new = webpage_new(URL, dep, HTML);
    return new;
}