#include "strHash.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>


// The hashing algorithm (dj2b), first found by Daniel J. Bernstein (https://en.wikipedia.org/wiki/Daniel_J._Bernstein)
// Pretty fast, and there is not a restriction on string size.
unsigned long prehash(char* string, strHashTable* table){
    unsigned long hash = 5381;
    int c;

    while ((c = *string++))
        hash = ((hash << 5) + hash) + c; 

    return hash; 

}

// Initialize a new hash table, returns a pointer to said hash table.
strHashTable* initHash(){
    strHashTable* new = (strHashTable*)malloc(sizeof(strHashTable));
    new->array = (collisionChain**)malloc(sizeof(collisionChain*)*INITIAL_TABLE_SIZE);
    memset(new->array,'\0',sizeof(collisionChain*)*INITIAL_TABLE_SIZE);
    new->keys = newWordPairList();                                  // use an expandable list of wordPairs to store all the keys for the table.
    assert(new != NULL && new->array != NULL && new->keys != NULL); // Verify the memory was allocated. exit if not.
    new->used = 0;                                                  // Number of elements filled in the array since the last resize.
    new->size = INITIAL_TABLE_SIZE;
    new->collisions = 0;                                            // The number of collisions that have occured since the last resize.
    return new;
}

// Adds a new wordpair to the hash table, returns the hash value of the string contained in wp.

int addHashW(char* w1, char* w2, strHashTable* table){
    return addHash(newWordPair(w1, w2), table);
}

wordPair* getWordPairP(wordPair* key, strHashTable* table){
    collisionChain* list = table->array[key->hash];
    if(list == NULL || key == NULL){
        return NULL; // key not found.
    }else{
        collisionChain* current = list;
        // Still have to loop through the collision chains and compare the strings (ew).
        do{
            if(key->prehash == current->pair->prehash && compareStr(current->pair->words, key->words)) return current->pair;
        }while((current = current->next) != NULL);
        return NULL;
    }
}

// Add a new word pair to the hash table.
// If the word collides, the hash may resize in order to maintain a certain ratio of collisions to insertions.
int addHash(wordPair* wp, strHashTable* table){ 
    wordPair* temp;
    wp->prehash = prehash(wp->words, table);
    wp->hash = wp->prehash%table->size;
    if((temp = getWordPairP(wp, table)) != NULL){
        temp->freq++;
        destroyWordPair(&wp);   // Duplicate, can free;
        return 0;  
    }else{ 
        if(table->array[wp->hash] != NULL){ // Collision case
            table->collisions++;
            addCollisionChainP(wp,&(table->array[wp->hash]));
            if(collisionRate(table) > REHASH_POINT){
                //printf("Resizing\n");  // Uncomment to see each time it resizes (excluding resizing done during rehash).
                // resize and rehash.
                int i = table->size;
                table->size *= MULTIPLIER;
                table->array = realloc(table->array, sizeof(collisionChain*)*table->size);
                assert(table->array != NULL);
                memset(&(table->array[i]),'\0',sizeof(collisionChain*)*(table->size-i));
                rehash(table);
            }
        }else{
            table->array[wp->hash] = newCollisionChainP(wp);
            table->used++;
        }
        pushWordPair(wp, table->keys);

    }
    return wp->hash;  
}
// This is a version of the above addHash function, but optomized for rehashing of an existing key.
// Not meant for use outside of the rehash function.
int reAddHash(wordPair* wp, strHashTable* table){ 
    wp->hash = wp->prehash%table->size;
    if(table->array[wp->hash] != NULL){ // Collision case
        table->collisions++;
        addCollisionChainP(wp,&(table->array[wp->hash]));
        if(collisionRate(table) > REHASH_POINT){
            //printf("Resizing\n");  // Uncomment to see each time it resizes during a rehash.
            // resize and rehash.
            int i = table->size;
            table->size *= MULTIPLIER;
            table->array = realloc(table->array, sizeof(collisionChain*)*table->size);
            assert(table->array != NULL);
            memset(&(table->array[i]),'\0',sizeof(collisionChain*)*(table->size-i));
            /*for(;i < table->size; i++){
                table->array[i] = NULL;
            }*/
            rehash(table);
        }
    }else{
        table->array[wp->hash] = newCollisionChainP(wp);
        table->used++;
    }

    return wp->hash;  
}

void rehash(strHashTable* table){
    unsigned long long oldHash; // used to store rehashed string values.
    wordPair* currentKey;
    table->collisions = 0;
    table->used = 0;
    for(int i = 0; i < table->keys->used; i++){
        currentKey = table->keys->array[i];
        oldHash = currentKey->hash;
        assert(removeWordPairCC(&(table->array[oldHash]), currentKey));
        reAddHash(currentKey,table);
    }

}
// Frees ALL memory associated with the hash table.
void destroyHashTable(strHashTable* table){
    wordPair* current;
    for(int i = 0; i < table->keys->used; i++){
        current = table->keys->array[i];
        if(current == NULL) continue;
        destroyCollisionChain(&(table->array[current->hash]));
    }
    destroyWordPairList(&(table->keys));
    free(table->array);
    free(table);
}

// Returns the ratio of collisions to the total size of the table.
double collisionRate(strHashTable* table){
    /* Returns collisions/insertions
       As the table size increases, the less desirable a rehash becomes.
       So, it makes sense to have more strict requirements while the table size is small to reduce
       the number of strings that must be rehashed (That is if speed is a priority).
    */
    if(table->used > 0) return ((double)table->collisions)/((double)(table->size));
    //return table->collisions;
    return 0.0;

}

// This is probably the fastest way to access a particular wordpair
// So long as the hash function is fast.
wordPair* getWordPair(char* key, strHashTable* table){
    collisionChain* list = table->array[prehash(key,table)];
    if(list == NULL || key == NULL){
        return NULL; // key not found.
    }else{
        collisionChain* current = list;
        // Still have to loop through the collision chains and compare the strings (ew).
        do{
            if(compareStr(current->pair->words, key)) return current->pair;
        }while((current = current->next) != NULL);
        return NULL;
    }
}

void printTopH(int count, strHashTable* table){
    sortWordPairList(table->keys);
    printTop(count, table->keys);
}

