/*

pagedir.h - header file for pagedir.c

Jason Chen
CS 50 Lab 4
February 9, 2021

*/


#ifndef __PAGEDIR_H
#define __PAGEDIR_H
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include "../libcs50/webpage.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/bag.h"


/**************** pagedirExists ****************/
/* Returns whether the pathname provided as pageDirectory is indeed a directory
 * Caller provides:
 *   a character string pageDirectory
 * We return:
 *   true if the directory exists and is writable, false otherwise
 *  
 */
bool pagedirExists(char *pageDirectory);

/**************** pagefetcher ****************/
/* Returns true if the fetch was successful; otherwise, false. 
 * Caller provides:
 *   a webpage
 * Assumptions:
 *     1. page has been allocated by caller
 *     2. page->url contains the url to fetch
 *     3. page->html is NULL at call time
 * We return:
 *   True if the fetch was successful; otherwise, false.
 * Caller is responsible for:
 *   Freeing the webpage 
 * Note: please see description of webpage_fetch() in webpage.h for more details
 *  
 */
bool pagefetcher(webpage_t *webpage);

/**************** pagesaver ****************/
/* Outputs a page to the appropriate file.
 * Caller provides:
 *   a webpage, pathname of the pageDirectory, and ID of file
 * We return:
 *   nothing
 * Caller is responsible for:
 *   Freeing the webpage 
 */
void pagesaver(webpage_t *webpage, char *directory, int ID);


/**************** pagescanner ****************/
/* extracts URLs from a page and adds valid URLs to bag and hashtable
 * Caller provides:
 *   a webpage, a hashtable storing seen URLs, and bag storing unvisited webpages
 * We return:
 *   nothing
 * Caller is responsible for:
 *   Freeing the webpage, hashtable, and bag 
 */
void pagescanner(webpage_t *webpage, hashtable_t *ht, bag_t *bag);


/**************** loadpage ****************/
/* creates a new webpage based on the crawler file
 * Caller provides:
 *   the file created by the crawler
 * We return:
 *   a webpage
 * Caller is responsible for:
 *   Freeing the URL and HTML
 */
webpage_t *loadpage(FILE *file);

#endif // __PAGEDIR_H