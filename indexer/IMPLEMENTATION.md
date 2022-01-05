The indexer program takes in command line in the form `./indexer pageDirectory indexFilename`. The program utilizes functions from webpage.h, hashtable.h, and counters.h in libcs50 directory, as well as pagedir.h and index.h in common directory. 

# Inside the main() function 
1. Handles Input:
The program parses the command line. It checks if the URL is valid through `pagedirExists` and checks whether `indexFilename` is writable. The program prints an error message returns non-zer with any errors.

2. Initiates objects and variables:
The program initiates a hashtable of string URLs by calling `hashtable_new`. The hashtable stores words as key and pointers to counters of (ID, count). The hashtable also does not allow duplicates.

An int variable called ID is initialized and set to 1. ID keeps track of the pageDirectory name and is incremented by one after each iteration.
A character string called pagedir is created to hold the pathname of the directory

3. Loop through each file and add all appropriate words in the files to hashtable structure
We loop through every file, calling `loadpage` to create a webpage for the file and `indexCreate` to add words to the inverted-index structure. We close the file, free the webpage through `webpage_delete`, increment the ID by one, change the string to the next filename, and attempt to open the next file.

4. Save the inverted-index to a file
We call `indexSave`, passing in the hashtable and filename.

4. Cleanup:
Free hashtable and appropriate character strings, return 0.

# Functions

## loadpage

We use `freadlinep` to read and store the URL and depth from the first and second lines of the file respectively. `freadfilep` is called to read the rest of the file, which is stored in a character string, "HTML". We create and return a new webpage through `webpage_new`.

## indexCreate

`indexCreate` takes in a webpage, hashtable, and ID.

It reads every word in the webpage using `webpage_getNextWord`. If the word is at least three letters long, it normalizes the word through `normalizeWord`.

    The function then calls `hashtable_find` to look for the word. If the word is not found, we initalize a new counters set through `counters_new` and insert into the hashtable (`hashtable_insert`). We then call `counters_add`, which increments the count by one if the ID already exists, or creates a new (ID, count) pair with count 1 if the ID does not exist.

The word is freed before attempting to get the next word.

## indexSave

`indexSave` takes in a hashtable and filename. It opens the file and calls `hashtable_print`. A helper method that prints the counters is passed in. The file is then closed.

