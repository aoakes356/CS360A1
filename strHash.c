#include "strHash.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "getWord.h"
#include "crc64.h"

// Just using the given hash function modulo table size.
unsigned long long prehash(char* string, strHashTable* table){
    return crc64(string)%table->size;
}

// Initialize a new hash table, returns a pointer to said hash table.
strHashTable* initHash(){
    strHashTable* new = (strHashTable*)malloc(sizeof(strHashTable));
    new->array = (collisionChain**)malloc(sizeof(collisionChain*)*INITIAL_TABLE_SIZE);
    for(int i = 0; i < INITIAL_TABLE_SIZE; i++){
        new->array[i] = NULL;                   // Initialize table values to NULL.
    }
    new->keys = newWordPairList();  // use an expandable list of wordPairs to store all the keys for the table.
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
            if(collisionRate(table) > .01){
                //printf("Resizing\n");  // Uncomment to see each time it resizes.
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

int main(int argc, char** argv){
    // Check if it has a useable number of arguments.
    if(argc < 2){
        printf("Incorrect Usage\n wordpairs <-count> fileName1 <fileName2> <fileName3> ...\n");
        return 0;
    }

    int count = 0, fileCount = argc-1, start = argc;

    if(compareStr(argv[1],"-count")){
        if(argc < 4){
            printf("Specify at least one file path\n");
            return 0;
        }
        (sscanf(argv[2],"=%d",&count) || sscanf(argv[2],"%d",&count));
        fileCount -= 2;
    }else if(sscanf(argv[1],"%d",&count) || sscanf(argv[1],"-count=%d",&count)){
        if(argc < 3){
            printf("Specify at least one file path\n");
            return 0;
        }
        fileCount--;
    }

    start = argc-fileCount;

    strHashTable* table = initHash();
    FILE *in;
    char* word1 = NULL, *word2 = NULL;
    for(int i = 0; i < fileCount; i++){
        in = fopen(argv[start+i],"r");
        assert(in != NULL);
        if(word1 == NULL){
            word1 = getNextWord(in);
        }
        word2 = getNextWord(in);
        if(word1 == NULL || word2 == NULL){
            continue;
        }
        addHashW(word1, word2, table);
        free(word1);
        word1 = NULL;
        while((word1 = word2) != NULL && (word2 = getNextWord(in)) != NULL){
            addHashW(word1, word2, table);
            free(word1);
            word1 = NULL;
        }
        if(word2 == NULL && i == fileCount -1){
            free(word1);
            word1 = NULL;
        }
    }
    if(count > 0){
        printTopH(count,table);
    }else{
        printTopH(table->keys->used-1, table);
    }
    //printf("Collisions: %lu, Table Size: %lu\n",table->collisions, table->size); // Uncomment to see collisions and table size.
    destroyHashTable(table);
    fclose(in);
    return 0;
}


