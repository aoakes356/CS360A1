#ifndef STR_HASH_H
#define STR_HASH_H

#define INITIAL_TABLE_SIZE 47   // 47 is particularly fast for some reason. 
#define STR_SIZE 16 // Determines the initial size of all word pairs. 
#define REHASH_POINT .1 // this seems to be the best rate despite resulting in more collisions, rehashing is still more expensive.

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

// Gets the value of the index of a given string for the hashtable
unsigned long long prehash(char*, strHashTable*);

// Initialize a new hash table
strHashTable* initHash();

// Add a new wordpair to the hash table.
int addHash(wordPair*, strHashTable*);

// Check the rate at which there are collisions in the hash table.
double collisionRate(strHashTable* );

// Re-calculate the hash values for each word pair already in the hash table.
void rehash(strHashTable* );

// Free all memory associated with a hash table.
void destroyHashTable(strHashTable*);

// Print a given quantity of elements from the front of the list of keys
void printTopH(int, strHashTable*);

// Given a string, find the associated wordpair in the hash table.
wordPair* getWordPair(char*, strHashTable*);

/* wordPair code */

// concatenate two strings together, resizing the destination string as needed.
// The given destination string is expected to be allocated using malloc().
int concat(char** dest, char* src, int destSize);

// allocate a new wordpair structure.
wordPair* newWordPair(char*, char*); // Expects strings allocated using malloc

// destroy a wordpair freeing all associated memory.
void destroyWordPair(wordPair**);

// increment the frequency variable in a given wordpair.
void incrementWordPair(wordPair*);

// Compare two word pairs, returns 1 if strings are equal 0 otherwise.
int equalsWordPair(wordPair*, wordPair*);

// Compares two strings.
int compareStr(char* str1, char* str2);

/* wordPairList code - used to store the keys used in the hashtable */

wordPairList* newWordPairList(); // Initialize a new wordPairList.

// push a new word pair into the list of word pairs.
void pushWordPair(wordPair*, wordPairList*);

// Remove the wordpair 
int removeWordPair(wordPair*, wordPairList*);

// Remove the wordPair at the given index.
int removeWordPairI(int, wordPairList*);

// Free the memory associated with the word pair list, leaving the word pairs themselves to be freed later.
void destroyWordPairList(wordPairList**);

// Sort the given word pair list using qsort.
void sortWordPairList(wordPairList*);

// print the top elements of the wordpair list.
void printTop(int, wordPairList*);

/* collisionChain code */

// create a new collision chain node using two strings
collisionChain* newCollisionChainW(char *, char*);

// Create a new collision chain node using a wordpair
collisionChain* newCollisionChainP(wordPair* );

// Destroy ALL memory associated with a collision chain.
void destroyCollisionChain(collisionChain** );

// Destroy the collision chain, leaving the word pair to be freed later.
void destroyCollisionChainWP(collisionChain** );

// Add a new node using two strings
void addCollisionChainW(char*, char*, collisionChain** );

// Add a new node using a premade wordpair
void addCollisionChainP(wordPair*, collisionChain** );

// Add a new node using a premade node.
void addCollisionChainQ(collisionChain* new, collisionChain** head);

// Print an entire collision chain - for debugging purposes.
void printCollisionChain(collisionChain*);

// Remove the collision chain containing the given wordpair.
int removeWordPairCC(collisionChain**, wordPair*);

#endif
