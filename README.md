# Search Engine

A search engine that takes in a query from user and returns a set of documents found based on frequency of query appearance.

## Crawler

The crawler crawls the web and retrieves webpages starting from a “seed” URL. It parses the seed webpage, extracts any embedded URLs, then retrieves each of those pages recursively, limiting its exploration to a given “depth”. It writes each explored page with a unique document ID to a `pageDirectory`.

## Indexer

The indexer reads the document files produced by the crawler, builds an inverted-index data structure mapping each word to its (document ID, count) pair. It writes the index to a file, with each line providing the word and its (docID, count) pairs. 

## Querier

The querier reads the index file produced by the indexer, and page files produced by the crawler, and answers search queries submitted via stdin. It prints the set of documents found based on word appearance in decreasing order.

### Usage
To build, run `make`.

To clean up, run `make clean`.

