The TSE Querier is a standalone program that reads the index file produced by the TSE Crawler, and page files produced by the TSE Querier, and answers search queries submitted via stdin

### User interface

The indexer's only interface with the user is on the command-line; it must always have two arguments.

```bash
$ ./querier pageDirectory indexFilename
```

- `pageDirectory` is the directory where crawler stored the webpage files it retrieves, and 
- `indexFilename` is a file where the indexer writes the inverted-index

### Inputs and outputs

*Input:* the inputs are command-line parameters. The webpage directory for which to index is specified by the first command-line parameter and the file where the inverted-index is written specified by the second parameter. See the User Interface above.

*Output:* Prints a ranking of the resulting set of documents according to its score. Prints this set of documents in decreasing rank order; for each, list the score, document ID and URL. (Obtain the URL by reading the first line of the relevant document file from the pageDirectory.)


### Functional decomposition into modules

We anticipate the following modules or functions:

 1. *main*, which parses arguments and initializes other modules and processes input from stdin
 2. *hasNonletters*, which checks if query has any nonletters (other than whitespaces)
 3. *isQueryValid*, which returns if whether query is bounded by "and" or "or"
 4. *parseQuery* which in the query builds an array of words, using whitespaces as delimiter
 5. *interpretQuery* which interprets query based on precedence of operators
 6. *processAndSequence* which returns a counterset of the and-sequence
 7. *countersIntersect*, which builds the intersection of two counter
 8. *countersUnion* which merges two counter sets into a new set
 9. *normalizeWord* which converts all letters to lowercase
 10. *countWords* which counts number of words in query
 11. *queryValid* which returns whether query is bounded by "and" or "or"
 12. *indexLoad* which creates an inverted-index data structure
 13. *rankDocument* which rearranges array based on decreasing count and prints output
 14. *pagedirExists* which checks if the pageDirectory exists


And some helper modules that provide data structures:

  1. ***counters*** of ID as key and count of the word as counter
  2. ***hashtable*** of word as key and counters as item

### Pseudo code for logic/algorithmic flow

The indexer will run as follows:

1. start from a command line as shown in the User Interface
2. parse the command line, validate parameters, initialize other modules
3. Take input from stdin. While it is not NULL:
  
    3.1 Check is does not have any non-letters other than whitespaces. Normalize word.
  
    3.2 Parse query, building an array of words using whitespaces as delimiter
  
    3.3 If query does not start or end with "and" or "or," perform the following:
    
        3.3.1 Create an inverted-index structure with word as key and counter sets of (ID, count) pairs as item
    
        3.3.2 Interpret the query, building a counter set of matched IDs
    
        3.3.3 Sort and print the counter set in order of decreasing count
    
        3.3.4 Delete counters and hashtable
  
    3.4 Ask for next query



### Dataflow through modules

 1. *main* parses parameters and calls indexCreate.
 2. *pagedirExists* checks if the pageDirectory exists
 3. *freadlinep* takes input from stdin
 4. *hasNonLetters* returns whether query has any non-letters aside from spaces
 5. *normalizeWord* converts all letters to lowercase
 6. *countWords* counts number of words in query
 7. *parseQuery* builds the array of words, using whitespace as delimiter
 8. *queryValid* returns whether query is bounded by "and" or "or"
 9. *indexLoad* creates an inverted-index data structure
 10. *interpretQuery* interprets query and builds counter set of matched IDs
 11. *rankDocument* rearranges array based on decreasing count and prints output

### Major data structures

Three helper modules provide data structures:

 1. ***counters*** of ID and count strctures
 2. ***set*** of words (indirectly used by hashtable)
 3. ***hashtable*** of words as key and counters as items

Additional data structure that will be used:
 4. ***doc*** of int IDs and int counts
 5. ***counterspair*** holding two counter sets

### Testing plan

A sampling of tests that should be run:

-  Test the program with various forms of incorrect command-line arguments to ensure that its command-line parsing, and validation of those parameters, works correctly.
  - incorrect number of parameters
  - nonexisting pageDirectory
  - nonexisting indexfile
- Test the program with various form of incorrect queries to make sure the querier works correctly
  - starting with and/or
  - ending with and/ors
  - extra spaces/tabs
  - special characters and numbers
  - no input
  - words that don't match
  - a combination of matched words with and/ors




