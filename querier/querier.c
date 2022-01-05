/*
 *
 * querier.c
 * 
 * Jason Chen
 * CS50
 * 
 * The TSE Querier is a standalone program that reads the index file produced by the TSE Crawler, 
 * and page files produced by the TSE Querier, and answers search queries submitted via stdin
 * 
 * Usage: ./querier pageDirectory indexFilename
 * 
 */
 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h> 
#include "../libcs50/webpage.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"
#include "../common/pagedir.h"
#include "../common/word.h"
#include "../common/index.h"
#include "../libcs50/file.h"
#define _GNU_SOURCE


char *pageDirectory;
int numctrs;    // store number of counters in "result" ctrs
int arrayI; // index for array


// struct used when finding intersection
typedef struct counterspair {
    counters_t *ctrs2;
    counters_t *intersection;
} counterspair_t;


static counterspair_t *counterspair_new(counters_t *ctrs2, counters_t *intersection)
{
    counterspair_t *ctrspair = malloc(sizeof(counterspair_t));
    ctrspair->ctrs2 = ctrs2;
    ctrspair->intersection = intersection;
    return ctrspair;
}

// struct used when ranking documents
typedef struct doc {
    int docID;
    int count;
} doc_t;


hashtable_t *ht;
const int hashtable_slots = 200;

// counters union
void countersUnion(counters_t *ctrs1, counters_t *ctrs2, counters_t *new);
void countersDuplicate(void *arg, const int key, const int count);
void countersMerge(void *arg, const int key, const int count);

// counters intersect
void countersIntersect(counters_t *ctrs1, counters_t *ctrs2, counters_t *intersection);
void intersectionHelper(void *arg, const int key1, const int count1);

int countWords(char *input);
bool hasNonletters(char *input);

// handle query
void parseQuery(char *input, char *words[]);
bool queryValid(char *array[], int numWords);
void interpretQuery(hashtable_t *ht, counters_t *result, char *words[], int numwords);
counters_t *processAndSequence(hashtable_t *ht, counters_t *temp, char *words[], int* i, int numwords);

// document ranking functions
void rankDocuments(counters_t *result);
void docInsert(void *arg, const int key, const int count);
// static void insertHelper(doc_t *doc, int ID, int count);
void itemcount(void *arg, const int key, const int count);
void ctrsdelete(void *item);
int compareCount(const void *document1, const void *document2);
void printDocs(doc_t *docArray);

void prompt(void);

int main(int argc, char* argv[]) {

    // checks if parameters entered satisfy usage
    if (argc != 3) {
        printf("Usage: ./querier pageDirectory indexFilename \n");
        return 1;
    }

    // check if first argument is a valid directory
    pageDirectory = argv[1];
    if (!pagedirExists(pageDirectory)) {
        printf("directory does not exist \n");
        return 1;
    }

    // check if second argument is a readable file
    char *indexfile = argv[2];
    FILE *ifile = fopen(indexfile, "r");

    if (ifile == 0) {
        printf("index file unsucessfully opened\n");
        return 1;
    } else {
        fclose(ifile);
    }

    // take and process query from user
    char *line;
    printf("Query?\n");
    while ((line = freadlinep(stdin)) != NULL) {
        int numwords;

        printf("Query: %s\n", line);

        // if query has nonletters other than spaces and tabs, skip and ask for another query
        if (hasNonletters(line) == 0) {
            free(line);
            printf("Invalid Query: non-letters found\n");
            continue;
        }

        line = normalizeWord(line); // convert any uppercase letters to lowercase

        // an array of words in query
        numwords = countWords(line);
        char *wordsArray[numwords];

        parseQuery(line, wordsArray);   // build the array of words, using whitespaces as delimiter

        // if the query is valid (no "and" or "or" in beginning or end)
        if (queryValid(wordsArray, numwords) == 1) {

            hashtable_t *ht = hashtable_new(200); 
            if (ht == NULL) {
                printf("Error creating hashtable\n");
                return 1;
            }


            counters_t *ctrs = counters_new();  // store IDs and counts of search result
            if (ctrs == NULL) {
                printf("Error creating counters set\n");
                return 1;
            }

            indexLoad(ht, indexfile);   // create the inverted index data structure

            interpretQuery(ht, ctrs, wordsArray, numwords); // interprets the query and builds counter set of matches

            rankDocuments(ctrs);    // rank docs by decreasing count given ctrs

            counters_delete(ctrs);
            hashtable_delete(ht, *ctrsdelete);

        }
    
        free(line);

        printf("----------------------------\n");

        prompt();


    }


    return 0;
}

// ranks the results counters based on decreasing occurance of docIDs
void rankDocuments(counters_t *result) {

    arrayI = 0;
    numctrs = 0;

    // find the number of counters in the "result" counters set
    counters_iterate(result, NULL, itemcount);

    if (numctrs == 0) {
        printf("No documents match\n");
    } else {


        doc_t *docArray = calloc(numctrs, sizeof(doc_t));

        // add all doc structs into the doc array
        counters_iterate(result, docArray, docInsert);

        // sort the docs in array based on decreasing count
        qsort(docArray, numctrs, sizeof(doc_t), compareCount);

        printf("Matches %d documents (ranked)\n", numctrs);

        printDocs(docArray);    // print out the scores, doc ID, and URL

        free(docArray);

    }
}


// print out the count, doc, and URL of docs in the array
void printDocs(doc_t *docArray) {


    for (int i=0; i<numctrs; i++) {

        // find the URL of the document by calling freadline
        char *dir = malloc(strlen(pageDirectory) + 1);
        strcpy(dir, pageDirectory);
        char *file = malloc(strlen(pageDirectory) + strlen("/1000") + 2);
        sprintf(file, "%s/%d", dir, docArray[i].docID);
        FILE *fpointer = fopen(file, "r");   
        char *URL = freadlinep(fpointer);

        fclose(fpointer);
        free(dir);
        free(file);

        printf("score:  %d doc  %d: %s\n", docArray[i].count, docArray[i].docID, URL);

        free(URL);
    
    }
}

// returns the whether count of doc2 is larger than doc1
int compareCount(const void *document1, const void *document2) {
    doc_t *doc1 = (doc_t*)document1;
    doc_t *doc2 = (doc_t*)document2;

    return(doc2->count - doc1->count);
}


// helper function called by counters_iterate to insert doc into array
void docInsert(void *arg, const int key, const int count) {

    doc_t *docArray = arg;

    // create struct holding docID and count 
    doc_t *doc = malloc(sizeof(doc_t));
    if (doc == NULL) {
        return;
    }
    doc->docID = key;
    doc->count = count;
    
    // set the index in array to point at doc structure
    docArray[arrayI] = *doc;

    arrayI++;   // increment index array by one

    free(doc);

}


// // helps insert doc into array
// static void insertHelper(doc_t *doc, int ID, int count)
// {
//     doc->docID = ID;
//     doc->count = count;
// }


// helper function to count items in counter
void itemcount(void *arg, const int key, const int count) {
    // int *a = arg;
    // (*numctrs)++;
    numctrs++;
}


// searches through data structure for matches based on query, and stores result in counters set
void interpretQuery(hashtable_t *ht, counters_t *result, char *words[], int numwords) {

    int a = 0;  // int used to access words in array
    int *i;
    i = &a;


    while (*i <= numwords-1) {

        counters_t *temp = counters_new();

        countersUnion(temp, hashtable_find(ht, words[*i]), temp);

        // check if the word is part of an and-sequence, and store in temp
        temp = processAndSequence(ht, temp, words, i, numwords);
        (*i) += 2;

        countersUnion(result, temp, result);    // merge result with temp 

        counters_delete(temp);

    }
}


// Processes and-sequences of query given starting word. Does nothing if the word is not in an and-sequence.
// If and sequence is found, adds intersecting counters to temp, and calls recursively, passing in the next word
counters_t *processAndSequence(hashtable_t *ht, counters_t *temp, char *words[], int* i, int numwords) {


    char *and = "and";
    char *or = "or";


    // if *i in range and next word is not or
    if (*i < numwords-1 && strcmp(words[*i+1], or) != 0) {

        // if the next word is "and,"" increment *i by 2
        if (*i < numwords-2 && strcmp(words[*i+1], and) == 0) {
            (*i) += 2;
        } else {    // if next word is not "or" or "and", increment *i by 1
            (*i)++;
        } 

        // find intersection of temp and counters at words[i], and call recursively
        counters_t *intersection = counters_new();
        countersIntersect(temp, hashtable_find(ht, words[*i]), intersection);
        counters_delete(temp);
        return processAndSequence(ht, intersection, words, i, numwords);
    }
    return temp;
}



// delete a counter set
void ctrsdelete(void *item)
{
  if (item != NULL) {

    counters_t *ctrs = item;
    counters_delete(ctrs);   
  }
}


// returns whether first or last word is "and" or "or"
bool queryValid(char *words[], int numWords) {
    char *and = "and";
    char *or = "or";
    // if the first or last is "and" return false
    if (strcmp(words[0], and) == 0) {
        printf("Invalid Query: first word cannot be \"and\"\n");
        return false;
    }
    if (strcmp(words[numWords - 1], and) == 0) {
        printf("Invalid Query: last word cannot be \"and\"\n");
        return false;
    }
    // if the first or last word is "or" return false
    if (strcmp(words[0], or) == 0) {
        printf("Invalid Query: first word cannot be \"or\"\n");
        return false;
    }
    if (strcmp(words[numWords - 1], or) == 0){
        printf("Invalid Query: last word cannot be \"or\"\n");
        return false;
    }
    
    return true;
}



// takes a string and builds an array of words, using whitespaces as delimiter
void parseQuery(char *input, char *words[]) {

    char *word; // pointer to first letter of word
    char *rest; // pointer to end of word

    int i = 0;
    while (*input != '\0') {
        word = input;
        rest = input;

        // move pointers to beginning of word
        while (!isalpha(*input) && *input != '\0') {
            input++;
            word++;
            rest++;
        }
        
        
        // move rest pointer to end of word
        while(isalpha(*input) && *input != '\0') {
            input++;
            rest++;
        }

        *rest = '\0';

        // move rest and input to letter after NULL
        rest++;
        input++;

        words[i] = word;     // index in array points to start of word
        i++;
        word = rest;

    }

}

// returns whether query has any nonletters (other than spaces, tabs, etc)
bool hasNonletters(char *input) {
    while (*input != '\0') {    // loop through every character
        if (!isalpha(*input)) {
            if (*input != ' ' && *input != '\n' && *input != '\t') {
                return false;
            }
        }
        input++;
    }
    return true;
}

// returns the number of words in query
int countWords(char *input) {

    int words = 0;
    int letter = 0; // track if previous character was letter


    while (*input != '\0') {

        // if current letter is letter and previous was not, increment word by one
        if (isalpha(*input) && (letter == 0)) {
            words++;
            letter = 1;
        // set letter to zero (acting as a boolean) if white space encountered
        } else if (*input == ' ' || *input == '\n' || *input == '\t') {
            letter = 0;
        }
        input++;

    }

    return words;
}


// function that returns a counters set with the union of two counters set
void countersUnion(counters_t *ctrs1, counters_t *ctrs2, counters_t *new) {
    
    // counters_t *new = counters_new();

    if (new == NULL) {
        return;
    }

    if (ctrs1 == NULL && ctrs2 != NULL) {
        counters_iterate(ctrs2, new, *countersDuplicate);   // copy ctrs2 to a new counter if ctrs1 is NULL
    } else {

        counters_iterate(ctrs1, new, *countersDuplicate);   // copy ctrs1 to a new counter
        counters_iterate(ctrs2, new, *countersMerge);   // merge counter2 with new counter
    
    }


}


// helper function to create a copy of a counters
void countersDuplicate(void *arg, const int key, const int count) {
    counters_t *new = arg;
    counters_set(new, key, count);
}


/* Consider one item for insertion into the other counters.
 * If the other counters does not contain the item, insert it;
 * otherwise, update the other counters's item with sum of item values.
 */
void countersMerge(void *arg, const int key, const int count) {
    counters_t *new = arg;

    int newcount = counters_get(new, key);
    if (newcount == 0) { // not found: insert it

        counters_set(new, key, count);

    } else {
        // add to the existing value
        newcount += count;
        counters_set(new, key, newcount);
    }
}


// function that returns a counters set with the intersection of two counters set
void countersIntersect(counters_t *ctrs1, counters_t *ctrs2, counters_t *intersection) {

    counterspair_t *pair = counterspair_new(ctrs2, intersection);   // struct holds two counter sets
    counters_iterate(ctrs1, pair, intersectionHelper);  // iterate through counter 1 and find intersections

    // free
    pair->ctrs2 = NULL;
    pair->intersection = NULL;
    free(pair);

}

// checks if two counters share a key. If so, then create a counters with that key and the minimum count of the two
void intersectionHelper(void *arg, const int key1, const int count1) {
    counterspair_t *pair = arg;

    int count2 = counters_get(pair->ctrs2, key1);
    // if the two counters share a key, then set key of "intersection" ctr to the minimum of the keys
    if (count2 != 0) {
        if (count2 < count1) {
            counters_set(pair->intersection, key1, count2);
        } else {
            counters_set(pair->intersection, key1, count1);
        }
    }
}



/* The fileno() function is provided by stdio,                                  
 * but for some reason not declared by stdio.h, so declare here.                
 */

int fileno(FILE *stream);
void prompt(void)
{
  // print a prompt iff stdin is a tty (terminal)
  if (isatty(fileno(stdin))) {
    printf("Query? ");
  }
}
