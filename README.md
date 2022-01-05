# Search Engine

## Crawler

The Crawler crawls the web and retrieves webpages starting from a “seed” URL. It parses the seed webpage, extracts any embedded URLs, then retrieves each of those pages recursively, limiting its exploration to a given “depth”. It writes each explored page with a unique document ID to a `pageDirectory`.

## Indexer

The Indexer reads the document files produced by the TSE crawler, builds an inverted-index data structure mapping each words to its document ID and count. It writes the index to a file, each line providing the word and its (docID, count) pairs. 

## Querier

The Querier reads the index file produced by the TSE Indexer, and page files produced by the TSE Crawler, and answers search queries submitted via stdin. It prints the set of documents found based on decreasing order of word appearance.

### Usage
To build, run `make`.

To clean up, run `make clean`.

