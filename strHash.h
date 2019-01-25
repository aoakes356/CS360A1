#ifndef STR_HASH_H
#define STR_HASH_H

#define INITIAL_TABLE_SIZE 256

#include <stdlib.h>

// Word pair structure for counting occurrences of word pairs.
typedef struct wordPair{
    char* words;
    unsigned long freq;    
    unsigned long hash;
} wordPair;

// Linked List of word pairs
typedef struct collisionChain{
    wordPair* pair;
    struct collisionChain* next;
} collisionChain;

// Hash table data structure
typedef struct strHashTable{
    collisionChain** array;
    size_t used;        
    size_t size;
    int collisions;
} strHashTable;

/* Hash table code */

unsigned long prehash(char*, unsigned long, unsigned long);

strHashTable* initHash();

int addHash(wordPair*, strHashTable*);

double collisionRate(strHashTable* );

void rehash(strHashTable* );

void destroyHashTable(strHashTable*);

wordPair* getMax(strHashTable*);

/* wordPair code */

wordPair* newWordPair(char*, char*); // Expects strings allocated using malloc

void destroyWordPair(wordPair* );

void incrementWordPair(wordPair*);

int equalsWordPair(wordPair*, wordPair*);

int compareStr(char* str1, char* str2);

/* collisionChain code */

collisionChain* newCollisionChainW(char *, char*);

collisionChain* newCollisionChainP(wordPair* );

void destroyCollisionChain(collisionChain** );

void addCollisionChainW(char*, char*, collisionChain** );

void addCollisionChainP(wordPair*, collisionChain** );

void addCollisionChainQ(collisionChain* new, collisionChain** head);

void printCollisionChain(collisionChain*);

#endif
