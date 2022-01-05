#  Jason Chen
## CS50 Lab5
## February 17, 2021


## Usage
Pagedir exports the following functions:

```c
bool pagedirExists(char *pageDirectory);
bool pagefetcher(webpage_t *webpage);
void pagesaver(webpage_t *webpage, char *directory, int ID);
void pagescanner(webpage_t *webpage, hashtable_t *ht, bag_t *bag);
webpage_t *loadpage(FILE *file);

```

Index exports the following functions:

```c
void indexCreate(webpage_t *page, hashtable_t *ht, int ID);
void indexLoad(hashtable_t *ht, char *oldIndexFile);
void indexSave(hashtable_t *ht, char *filename);

```

Words exports the following function:

```c
char *normalizeWord(char *word);

```

### Compilation
• to compile, simply `make`