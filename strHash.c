#include "strHash.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "getWord.h"
#include "crc64.h"
/* This hash/prehash implementation is based on an MIT open course video 
 * (https://www.youtube.com/watch?v=0M_kIqhwbFo).
 * The probability of collision is 1/m in the worst case, where m is the size of
 * the table. The method is known as 'Universal Hashing'.
 * h(k) = [(a*k +b)mod p] mod m
 * This is not a perfect implementation though. To insure that k was unique for
 * each string, I would need a really big integer (More than unsigned long)
 * or a constrained word size of 8, so I made this wierd recursive thing just to test it out.
 * 
 */


/*unsigned long prehash(char* string, unsigned long b, unsigned long tableSize){
    if(*string == '\0') return b;
    return prehash(string+1,((31*(*string) + b)%(1000000009))%tableSize, tableSize);

    
}*/

unsigned long long prehash(char* string, strHashTable* table){
    return crc64(string)%table->size;
}

strHashTable* initHash(){
    strHashTable* new = (strHashTable*)malloc(sizeof(strHashTable));
    new->array = (collisionChain**)malloc(sizeof(collisionChain*)*INITIAL_TABLE_SIZE);
    for(int i = 0; i < INITIAL_TABLE_SIZE; i++){
        new->array[i] = NULL;
    }
    new->keys = newWordPairList();
    assert(new != NULL && new->array != NULL && new->keys != NULL);  // Verify the memory was allocated. exit if not.
    new->used = 0;  // Number of elements filled in the array since the last resize.
    new->size = INITIAL_TABLE_SIZE;
    new->collisions = 0;    // The number of collisions that have occured since the last resize.
    return new;
}

// Adds a new wordpair to the hash table, returns 0 if there is a collision, else returns 1.

int addHashW(char* w1, char* w2, strHashTable* table){
    return addHash(newWordPair(w1, w2), table);
}
// Add a new word pair to the hash table.
// If the word collides, the hash may resize in order to maintain a certain ratio of collisions to insertions.
int addHash(wordPair* wp, strHashTable* table){ 
    wordPair* temp;
    unsigned long hash = prehash(wp->words, table);
    wp->hash = hash;
    if((temp = getWordPair(wp->words, table)) != NULL){
        temp->freq++;
        destroyWordPair(&wp);   // Duplicate, can free;
    }else{ 
        if(table->array[hash] != NULL){ // Collision case
            table->collisions++;
            addCollisionChainP(wp,&(table->array[hash]));
            if(table->collisions > table->size){
                // resize and rehash.
                int i = table->size;
                table->size *= 3;
                table->array = realloc(table->array, sizeof(collisionChain*)*table->size);
                assert(table->array != NULL);
                for(;i < table->size; i++){
                    table->array[i] = NULL;
                }
                rehash(table);
            }
        }else{
            table->array[hash] = newCollisionChainP(wp);
            table->used++;
        }
        pushWordPair(wp, table->keys);
    }
    return hash;  
}

void rehash(strHashTable* table){
    unsigned long long oldHash; // used to store rehashed string values.
    wordPair* currentKey;
    table->collisions = 0;
    table->used = 0;
    for(int i = 0; i < table->keys->used; i++){
        currentKey = table->keys->array[i];
        oldHash = currentKey->hash;
        if(table->array[oldHash] != NULL){
            assert(removeWordPairCC(&(table->array[oldHash]), currentKey));
            assert(removeWordPair(currentKey, table->keys));
            addHash(currentKey,table);
        }
    }
   
}
// Frees ALL memory associated with the hash table.
void destroyHashTable(strHashTable* table){
    collisionChain* current;
    /* needs to be refactored to use the keys in the wordpairlist */
    for(int i = 0; i < table->size; i++){
        if((current = table->array[i]) == NULL) continue;
        destroyCollisionChain(&current);
    }
    destroyWordPairList(&(table->keys));
    free(table->array);
    free(table);
}

double collisionRate(strHashTable* table){
    // Returns collisions/insertions
    if(table->used > 0) return ((double)table->collisions)/((double)table->size);
    return 0.0;
    
}

wordPair* getMax(strHashTable* table){
    wordPair* max;
    long unsigned maxCount = 0;
    for(int i = 0; i < table->size; i++){
        if(table->array[i] == NULL) continue;
        collisionChain* current = table->array[i];
        do{
            if(current->pair->freq> maxCount){
                maxCount = current->pair->freq;
                max = current->pair;
            }
        }while((current = current->next) != NULL);
    }
    return max;
}

wordPair* getWordPair(char* key, strHashTable* table){
    collisionChain* list = table->array[prehash(key,table)];
    if(list == NULL || key == NULL){
        return NULL; // key not found.
    }else{
        collisionChain* current = list;
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

int main(){
    /*printf("%ld\n",prehash("a",0 ,45733));
    printf("-------Expected-------\n1\n0\n0\n1\n-------Actual-------\n");
    printf("%i\n",compareStr("Hello", "Hello"));
    printf("%i\n",compareStr("Hello1", "Hello"));
    char* string1 = (char*)malloc(sizeof(char)*50);
    char* string2 = (char*)malloc(sizeof(char)*50);
    char* string3 = (char*)malloc(sizeof(char)*50);
    char* string4 = (char*)malloc(sizeof(char)*50);
    char* string5 = (char*)malloc(sizeof(char)*50);
    char* string6 = (char*)malloc(sizeof(char)*50);
    strcpy(string1,"Hello");
    strcpy(string2,"Frank");
    strcpy(string3,"Hello");
    strcpy(string4,"Bob");
    strcpy(string5,"Hello");
    strcpy(string6,"Frank");
    wordPair* wp1 = newWordPair(string1, string2);
    wordPair* wp2 = newWordPair(string3, string4);
    wordPair* wp3 = newWordPair(string5, string6);
    printf("%i\n",equalsWordPair(wp1,wp2));
    printf("%i\n",equalsWordPair(wp1,wp3));
    destroyWordPair(wp1);
    destroyWordPair(wp2);
    destroyWordPair(wp3);
    collisionChain* newList = newCollisionChainP(wp1);
    addCollisionChainP(wp2,&newList); 
    addCollisionChainP(wp3,&newList); 
    collisionChain* trav = newList;
    do{
        printf("%s %s\n",trav->pair->word1,trav->pair->word2);
    }while((trav = trav->next) != NULL);

    destroyCollisionChain(&newList); 
    if(newList == NULL){
        printf("Good!\n");
    }*/
    FILE *in = fopen("mobydick.txt","r");
    strHashTable* table = initHash();
    char *word1 = getNextWord(in);
    char *word2 = getNextWord(in);
    if(word1 == NULL || word2 == NULL){
        return 0;
    }
    addHashW(word1, word2, table);
    free(word1);
    word1 = NULL;
    while((word1 = word2) != NULL && (word2 = getNextWord(in)) != NULL){
        //printf("%s %s\n",word1, word2);
        addHashW(word1, word2, table);
        free(word1);
    }
    if(word2 == NULL){
        free(word1);
    }
    printTopH(10,table);
    destroyHashTable(table);
    fclose(in);
    return 0;
}


