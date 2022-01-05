Querier is a standalone program that reads the index file produced by the TSE Crawler, and page files produced by the TSE Querier, and answers search queries submitted via stdin. The TSE Querier takes in command line in the form `./querier pageDirectory indexFilename`. The program utilizes functions from hashtable.h, and counters.h in libcs50 directory, as well as pagedir.h and index.h in common directory. 

# Inside the main() function 
1. Handles Input:
The program parses the command line. It checks if the URL is valid through `pagedirExists` and checks whether `indexFilename` is readable. The program prints an error message returns non-zero with any errors.

2. The program takes input from standard in through `freadlinep` and stores the query in a pointer to char.

3. We check if the query contains any non-letters other than whitespaces through `hasNonletters`.

4. The query is normalized through `normalizeWord`, a method in the word.h module.

5. The program counts the number of words in the query through `countWords`, then creates an array that points to character strings. 

6. We parse the query through `parseQuery`, building the array of words using whitespaces as delimiters.

7. We check if the first and last element in array is not "and" or "or." If the query is bounded by either "and" or "or," it is considered invalid, and we ask for the next prompt by calling `prompt`.

8. If the query is valid, we initiate a new hashtable and call `indexLoad` to build an inverted-index data structure. We also create a new counters set to store the IDs and counts of matching documents.

9. The program interprets the query based on the precendence of each word in query through `interpretQuery`. This function builds the counter set with data of matching documents.

10. The program ranks the documents based on decreasing count through `rankDocuments`.

11. Finally, the counters and hashtable are freed (if initiated), and next prompt is called. 


 The Design Spec describes abstractions - abstract data structures, and pseudocode. The same design could be implemented in C or Java or another language. The Implementation Spec gets more specific. It is language, operating system, and hardware dependent. The implementation spec includes many or all of these topics:
Detailed pseudo code for each of the objects/components/functions,
Definition of detailed APIs, interfaces, function prototypes and their parameters,
Data structures (e.g., struct names and members),
Security and privacy properties,
Error handling and recovery,
Resource management,
Persistant storage (files, database, etc)

# Functions

```c

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
```


## pagedirExists
We attempt to create a file in the directory and return whether the file was successfully inserted.

## indexLoad
We read each word in the file and store the corresponding docID and count in int variables. If a counters set not yet exists for that word, we create a new counters set and insert into the hashtable. We then set a new counters into that counters set with the docID and count.

## parseQuery
We declare several character pointers. `word` tracks the start of a word. `rest` tracks the end of a word, and changes the subsequent character to a NULL character. 

We loop through the input while it is not null. We point `word` and `rest` to the first character of the word. Then we loop through the characters and point `rest` at the end of the word and add a NULL character. The array points to the first letter of word, and the variables are updated to the position of `rest`. Repeat until the end of string.

## queryValid
We use `strcmp` to check if the first or last element in array matches "and" or "or." If so, return false, else, return true.

## interpretQuery
An pointer to int is used to track the index of the array.

First, we declare a `temp` counters set that is a duplicate of the counters set of words[i]. To create this duplicate, we call `countersUnion`.

We then call `processAndSequence` to check if that word is part of an and-sequence. We update the `temp` counters accordingly, including the resulting counterset of all words in the and-sequence.

We must increment the index of the counter by two after that, as the completion of `processAndSequence` suggests that the next word is an "or", which we do not want to include in our search. 

We call `countersUnion` again to merge `temp` with our `result` counter. 

This process is repeated until the end of the array is reached.

## processAndSequence
This is a recursive function that updates `temp` counters set based on the discovery of an and-sequence. We start with the word passed in. If the subsequent word is "and", we increment i by 2, as we want to process the word after the and. If the subsequent word is not "and" or "or", we are treating this as an implicit and-sequence, thus we increment i by 1 to process that word. We create a new temporary counters set called `intersection` that is the intersection of counters of the current word and the `temp` counters passed in. `processAndSequence` is called recursively. If the next word is "or", simply return `temp`.

## countersUnion
We create a new counters set that is a duplicate of one of the two passed in, depending on if any is NULL. We then call a helper method to merge the new counterset with the one that was not duplicated. `counters_iterate` is used in this process.

## counterIntersect
We create a new `counterspair` struct that holds two counters set, a new `intersection` counters set and one that is passed in. We call `counters_iterate` on the counters set that isn't included in the struct, and pass in the counterspair along. In a helper method, we compare the two passed-in counters sets, and if any intersection is found, add a new counters to the `intersection` counters with the minimum count of the two.

## rankDocuments
A data structure `doc` is used by this method. `Doc` holds an int docID and int count.

The method first calls counters_iterate to find the number of counters in the `result` counters. This is helpful when declaring the array holding struct docs. If the number of counters is zero, we simply print a statement indicating that.

If the counters set is not empty, we create a struct doc for each counters and add them into the array. To do this, we call counters_iterate and a helper method, docInsert. `docInsert` creates a new doc object for each counters and adds the doc into the array.

We then sort the docs in the array based on decreasing count. `qsort` is used to perform this.

Lastly, we have a function printDocs that prints out the docs in the array in the desired format. To do this, we can use a for loop, looping through all items in the array, and printing out the docID and count of each struct doc. We also need to print the URL, so we call `freadlinep` on the corresponding docID in the pageDirectory folder to obtain the URL.


# Data structures

## counterspair
A struct that holds two sets of counters, one of which being the intersection counters. Used by countersIntersect

## doc
A struct that stores in ID and count of a particular word. Used when ranking files.


# Errors

Error statements are printed in stdout. 
If the commandline is invalid, the program will return with non-zero status. 
If the query is invalid, the program will print an error statement and ask for next query.