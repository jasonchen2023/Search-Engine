The TSE indexer is a standalone program that reads the document files produced by the TSE crawler, builds an index, and writes that index to a file. Its companion, the index tester, loads an index file produced by the indexer and saves it to another file.

### User interface

The indexer's only interface with the user is on the command-line; it must always have two arguments.

```bash
$ indexer pageDirectory indexFilename
```

- `pageDirectory` is the directory where crawler stored the webpage files it retrieves, and 
- `indexFilename` is a file where the indexer writes the inverted-index

For example, to index the webpages stored in `dir` by the crawler and and write the inverted-index to an indexFilename `indexFile`, we would use this command-line:

``` bash
$ indexer ../crawler/dir indexFile
```

### Inputs and outputs

*Input:* the inputs are command-line parameters. The webpage directory for which to index is specified by the first command-line parameter and the file for which to write the index is specified by the second parameter. See the User Interface above.

*Output:* Indexer will save the index to a file `indexFilename`. the file shall be in the following format.

 * one line per word, one word per line
 * each line provides the word and one or more (docID, count) pairs, in the format:
 * word docID count [docID count]...
    * where word is a string of lower-case letters,
    * where docID is a positive non-zero integer,
    * where count is a positive non-zero integer,
    * where the word and integers are separated by spaces.


### Functional decomposition into modules

We anticipate the following modules or functions:

 1. *main*, which parses arguments and initializes other modules
 2. *loadpage*, which creates a webpage according to the file in `pageDirectory`
 3. *indexCreate* which adds each valid word in the webpage to the inverted-index structure
 4. *indexLoad* which creates an inverted index data structure based on the index file
 5. *indexSave* writes the inverted index data structure to a file

And some helper modules that provide data structures:

  1. ***counters*** of ID as key and count of the word as counter
  2. ***hashtable*** of word as key and counters as item

### Pseudo code for logic/algorithmic flow

The indexer will run as follows:

1. start from a command line as shown in the User Interface
2. parse the command line, validate parameters, initialize other modules
3. while there are more files in `pageDirectory` to read,
    3.1 Read the file and create a webpage item for each file
    3.2 while there are more words to read from the webpage
        3.2.1 ignore if the word is less than 3 letters
        3.2.2 normalize the word
        3.2.3 check if the hashtable contains that word
        3.2.4 If not, create a counters for the word and add to the hashtable
        3.2.5 Add the ID to the counters
    3.3 delete webpage and open next file
4. write the index to a file
5. clean up


**normalize the word** means to convert it into lower-case form.


### Dataflow through modules

 1. *main* parses parameters and calls indexCreate.
 2. *loadpage* returns a webpage according to file
 3. *indexCreate* creates the inverted index data structure with words as key, and counters of (ID, count) pairs as item
 5. *indexSave* writes the inverted index data structure to a file

### Major data structures

Three helper modules provide data structures:

 1. ***counters*** of ID and count strctures
 2. ***set*** of words (indirectly used by hashtable)
 3. ***hashtable*** of words

A data structure that will be used:

 4. *webpage* which contains all the data read for a given webpage, plus the URL and the depth at which it was fetched

### Testing plan

A sampling of tests that should be run:

-  Test the program with various forms of incorrect command-line arguments to ensure that its command-line parsing, and validation of those parameters, works correctly.

-  Call crawler to create `pageDirectory` to test indexer
-  Test with `pageDirectory` as well as various nonexisting files, invalid inputs, etc

