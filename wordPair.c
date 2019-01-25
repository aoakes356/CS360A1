#include "strHash.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// Create a new word pair.
wordPair* newWordPair(char* w1, char* w2){ // Expects strings allocated using malloc
    wordPair* new = (wordPair*)malloc(sizeof(wordPair));
    assert(new != NULL);
    new->word1 = w1;
    new->word2 = w2;
    new->freq = 1;      // Initial frequency is one of course;
    return new;
}

// Free memory used by a word pair.
void destroyWordPair(wordPair* w){
    free(w->word1);
    free(w->word2);
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

int equalsWordPair(wordPair* p1, wordPair* p2){
     if(compareStr(p1->word1,p2->word1) && compareStr(p1->word2, p2->word2)) return 1;
     return 0;
}
