The crawler program takes in command line in the form `./crawler URL pageDirectory maxdepth`. The program utilizes functions from the webpage.h file in libcs50 directory. 

# Inside the main() function 
1. Handles Input:
The program parses the command line. It checks if the URL is valid through `IsInternal()`, if the pageDirectory exists through `pagedirExists()`, and if the maxdepth is between 0 and 10. If any of these checks fail, then the program prints an error message returns non-zero.

2. Initiates objects and variables:
The program initiates a bag through `bag_new()`. The bag keeps track of webpages that have not yet been crawled. Since the bag does not allow for duplicates, this ensures that each webpage will only be crawled once. The program also initiates a hashtable of string URLs by calling `hashtable_new()`. The hashtable keeps track of already-visited URLs. The hashtable also does not allow duplicates.

An int variable called ID is initialized and set to 1. ID keeps track of the pageDirectory name and is incremented by one after the creation of a webpage.

3. Calls crawler:
The `crawler()` function is called and passed in the URL, hashtable, and bag.

4. Cleanup:
The hashtable and bag are freed in the end, and we return 0.

# Functions

## Crawler

The `crawler()` function takes in a URL, hashtable, and bag. It calls `webpage_new()` to create a new webpage for the URL with depth 0 and NULL HTML. This webpage is inserted into the hashtable and bag.

The function extracts webpages from the bag and performs the following on each webpage until the bag is empty:

  We use `page_fetcher()` to retrieve the HTML for the page. See below for the implementation of `page_fetcher()`. 

  If the webpage was successfully fetched, we use `pagesaver()` to write the webpage to the pageDirectory with a unique ID. See below for implementation of `pagesaver()`.
    
    If the depth of the webpage is less than the maxDepth, we explore the webpage to find links:

      We call `pagescanner()` to parse the webpage and extract embedded URLs one at a time. If the URL is successfully inserted into the hashtable, we insert it into the bag. See below for implementation of `pagescanner()`.
    
  The webpage is deleted through `webpage_delete()`.


Note: Below functions are stored in pagedir.c in common directory
## pagefetcher
• `pagefetcher()`– Takes in a webpage and calls `webpage_fetch()`, passing in the same webpage. Returns whether the page was successfully fetched or not.

## pagesaver
• `pagesaver()` – Takes in a webpage, string pageDirectory, and int ID. The function initializes a pointer to a FILE and a filename in the format 'dir/ID'. The function prints the URL on the first line by calling `webpage_getURL()`, the depth on the second line by calling `webpage_getDepth()`, and HTML on the third line by calling `webpage_getHTML()`. The file is closed and strings are freed.

## pagescanner
• `pagescanner()` – Takes in a webpage, hashtable, and bag. It uses `webpage_getNextURL()` to get the next URL from the webpage and stores it in a pointer to a string called 'result'. We normalize the URL through the function `NormalizeURL()`. If the URL is internal and successfully inserted to the hashtable, we create a new webpage with that URL. This webpage has a depth one greater than that of the webpage passed in and NULL HTML. Finally, the webpage is inserted into the bag. Appropriate messages are printed to inform whether the URL was successfully added, ignored due to duplication, or ignored due to externality. The string 'result' is freed after each URL assignment.

## pagedirExists
• `pagedirExists()` – Takes in a string denoting the name of the directory. It allocates memory to create a string in the format "dir/.crawler", which is the pathname of the file. The method uses fopen() attempt to write to that file. If successful, the function returns true. Otherwise, return false.