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

// An expandable array which stores word pairs.
typedef struct wordPairList{
    wordPair** array;
    size_t used;
    size_t size;
} wordPairList;

// Linked List of word pairs
typedef struct collisionChain{
    wordPair* pair;
    struct collisionChain* next;
} collisionChain;

// Hash table data structure
typedef struct strHashTable{
    collisionChain** array;
    wordPairList* keys;
    size_t used;        
    size_t size;
    int collisions;
} strHashTable;

/* Hash table code */

unsigned long long prehash(char*, strHashTable*);

strHashTable* initHash();

int addHash(wordPair*, strHashTable*);

double collisionRate(strHashTable* );

void rehash(strHashTable* );

void destroyHashTable(strHashTable*);

void printTopH(int, strHashTable*);

wordPair* getWordPair(char*, strHashTable*);

/* wordPair code */

int concat(char* dest, char* src, int destSize);

wordPair* newWordPair(char*, char*); // Expects strings allocated using malloc

void destroyWordPair(wordPair**);

void incrementWordPair(wordPair*);

int equalsWordPair(wordPair*, wordPair*);

int compareStr(char* str1, char* str2);

/* wordPairList code - used to store the keys used in the hashtable */

wordPairList* newWordPairList(); // Initialize a new wordPairList.

void pushWordPair(wordPair*, wordPairList*);

// Remove the wordpair 
int removeWordPair(wordPair*, wordPairList*);

// Remove the wordPair at the given index.
int removeWordPairI(int, wordPairList*);

void destroyWordPairList(wordPairList**);

void sortWordPairList(wordPairList*);

void printTop(int, wordPairList*);

/* collisionChain code */

collisionChain* newCollisionChainW(char *, char*);

collisionChain* newCollisionChainP(wordPair* );

void destroyCollisionChain(collisionChain** );

void destroyCollisionChainWP(collisionChain** );

// Add a new node using two strings
void addCollisionChainW(char*, char*, collisionChain** );

// Add a new node using a premade wordpair
void addCollisionChainP(wordPair*, collisionChain** );

// Add a new node using a premade node.
void addCollisionChainQ(collisionChain* new, collisionChain** head);

void printCollisionChain(collisionChain*);

int removeWordPairCC(collisionChain**, wordPair*);

#endif
