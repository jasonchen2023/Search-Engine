# Jason Chen
## CS50 Lab 4
## February 10, 2021

The crawler crawls a website and retrieves webpages starting with a specified URL. It parses the initial webpage, extracts any embedded URLs and retrieves those pages, and crawls the pages found at those URLs, but limits itself to maxDepth hops from the seed URL and to URLs that are ‘internal’ to the designated CS50 server. When the crawler process is complete, the indexing of the collected documents can begin.

Usage: `./crawler URL pageDirectory maxDepth`

### Assumptions

• `pagefetcher()`, `pagescanner()`, `pagesaver()`, and `dirnameExists()` located within pagedir.c in 'common' directory
• `webpage_getHTML()` results in segmentation fault with URL: 'http://cs50tse.cs.dartmouth.edu/tse/wikipedia/Yahoo%21.html' and possibly a few others from the wikipedia playground. For this reason, I've decided not to test with URL: 'http://cs50tse.cs.dartmouth.edu/tse/wikipedia/' with maxDepth=2 in my testing script. I believe the segmentation fault is resulting from `webpage_getHTML()` and not my crawler.c program.

### Compilation
• to compile, simply `make`