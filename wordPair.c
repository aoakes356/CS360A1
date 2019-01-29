#include "strHash.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

// Create a new word pair.
wordPair* newWordPair(char* w1, char* w2){ // Expects strings allocated using malloc
    wordPair* new = (wordPair*)malloc(sizeof(wordPair));
    assert(new != NULL);
    new->words = (char*)calloc(sizeof(char),64);
    assert(new->words != NULL);
    strncpy(new->words,"",64);
    // Make the two words into one word with a space inbetween.
    concat(new->words, w2, concat(new->words," ",concat(new->words,w1,64)));
    new->freq = 1;      // Initial count is one of course;
    return new;
}

// Free memory used by a word pair.
void destroyWordPair(wordPair** w){
    free((*w)->words);
    free(*w);
    *w = NULL;
}

void incrementWordPair(wordPair* w){
    w->freq++;
}

// Compare two strings.
int compareStr(char* str1, char* str2){ // Assumes null terminated string.
    int i;
    if(str1 == NULL || str2 == NULL){
        if(str1 == str2){
            return 1;
        }
        return 0;
    }
    // This is for fun mostly.
    for(i = 1; (i *= (str1[i-1] == str2[i-1]) ) && (str1[i-1] != '\0') && (str2[i-1] != '\0'); i++);
    return !!i; // Guarantees i is between 0 and 1;
}

// Concatenation which resizes the string as needed.
int concat(char* dest, char* src, int destSize){
    int i,j;
    assert(dest != NULL && src != NULL);
    for(i = 0; dest[i] != '\0'; i++); // Get the string length of dest.
    for(j = 0; src[j] != '\0'; j++){
        if(j >= destSize){  // Need to resize the destination string.
            destSize += 20;
            dest = (char* )realloc(dest,destSize*sizeof(char));
            assert(dest != NULL);
        }
        dest[j+i] = src[j];
    }
    dest[j+i] = '\0'; // Terminate the new string.
    return destSize;
}

int equalsWordPair(wordPair* p1, wordPair* p2){
     return compareStr(p1->words,p2->words);
}
