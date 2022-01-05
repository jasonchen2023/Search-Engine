/*
 * word.c - functions that format character strings
 * 
 * Jason Chen
 * CS 50, Lab5
 * 
 */



#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "word.h"

char *normalizeWord(char *word) {
    for(int i = 0; word[i]; i++){
        word[i] = tolower(word[i]);
    }
    return word;
}
