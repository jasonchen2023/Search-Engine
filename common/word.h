/*
 * word.h - header file for word.c
 * 
 * Jason Chen
 * CS50, Lab 5
 */



#ifndef __WORD_H
#define __WORD_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

/**************** normalizeWord ****************/
/* Returns the lowercased word 
 * Caller provides:
 *   a character string word
 */
char *normalizeWord(char *word);

#endif // __INDEX_H 