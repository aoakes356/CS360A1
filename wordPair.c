#include "strHash.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
// Create a new word pair.
wordPair* newWordPair(char* w1, char* w2){ // Expects strings allocated using malloc
    wordPair* new = (wordPair*)malloc(sizeof(wordPair));
    assert(new != NULL);
    new->words = (char*)malloc(sizeof(char)*64);
    strncat(w1,new->words,255);
    strncat(" ",new->words,2);
    strncat(w2,new->words,255);
    new->freq = 1;      // Initial count is one of course;
    return new;
}

// Free memory used by a word pair.
void destroyWordPair(wordPair* w){
    free(w->words);
    free(w);
}

void incrementWordPair(wordPair* w){
    w->freq++;
}

// Compare two strings.
int compareStr(char* str1, char* str2){ // Assumes null terminated string.
    int i;
    // This is for fun mostly.
    for(i = 1; (i *= (str1[i-1] == str2[i-1]) ) && (str1[i-1] != '\0') && (str2[i-1] != '\0'); i++);
    return !!i; // Guarantees i is between 0 and 1;
}

// Concatenation which resizes the string as needed.
void concat(char* dest, char* src, int destSize){
    int i;
    for(i = 0; dest[i] != '\0'; i++);
    for(int j = 0; src[j] != '\0'; j++){
        dest[j+i] = src[j];
    }
}

int equalsWordPair(wordPair* p1, wordPair* p2){
     if(compareStr(p1->word1,p2->word1) && compareStr(p1->word2, p2->word2)) return 1;
     return 0;
}
