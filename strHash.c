#include "strHash.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "getWord.h"
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


unsigned long prehash(char* string, unsigned long b, unsigned long tableSize){
    if(*string == '\0') return b;
    return prehash(string+1,((31*(*string) + b)%(1000000009))%tableSize, tableSize);

    
}

strHashTable* initHash(){
    strHashTable* new = (strHashTable*)malloc(sizeof(strHashTable));
    new->array = (collisionChain**)malloc(sizeof(collisionChain*)*INITIAL_TABLE_SIZE);
    assert(new != NULL && new->array != NULL);  // Verify the memory was allocated. exit if not.
    for(int i = 0; i < INITIAL_TABLE_SIZE; i++){
        new->array[i] = NULL; // Initialize all elements in the array to NULL;
    }
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
    unsigned long hash = prehash(wp->word2,prehash(wp->word1,0,table->size),table->size);
    //printf("%lu\n",hash);
    wp->hash = hash;
    if(table->array[hash] == NULL){
        table->used++;
        table->array[hash] = newCollisionChainP(wp);
        return 0;
    }else{
        // Check if the same pair exists in the linked list already.
        collisionChain* current = table->array[hash], *prev = NULL;
        collisionChain* new;
        do{
            if(equalsWordPair(wp,current->pair)){   // Compare each word pair
                wp->freq = current->pair->freq+1;   // Swap the old chain with the new one
                new = newCollisionChainP(wp);
                new->next = current->next;
                if(prev != NULL){
                    prev->next = new;
                }else{
                    table->array[hash] = new;
                }
                current->next = NULL;
                destroyCollisionChain(&current);    // Free the old chain
                // This is done in order to ensure that duplicate word pairs are freed properly.
                return 1;
            }
            prev = current;
        }while((current = current->next) != NULL);
        // Since it's not a duplicate, it's a new collision
        table->collisions++;
        addCollisionChainP(wp,&table->array[hash]);
        
        if(collisionRate(table) > .01){
            // Resize
            printf("Resizing\n");
            //printf("Resizing the hash table.\n");
            size_t temp = table->size;
            table->size *= 3;
            table->array = (collisionChain**)realloc(table->array,sizeof(collisionChain*)*table->size);
            for(int i = temp-1; i < table->size; i++){
                table->array[i] = NULL; // Set the new pointers to NULL.
            }
            // Rehash
            rehash(table);
            table->used = 0;
            table->collisions = 0;
        }
        return 0;
    }
}

void rehash(strHashTable* table){
    unsigned long temp;
    collisionChain* current, *prev;
    for(int i = 0; i < table->size; i++){
        current = table->array[i];
        prev = NULL;
        //printf("rehash iteration: %i\n",i);
        if(current == NULL) continue;               // Loop until non empty linked list is found.
  //      printCollisionChain(current);
        do{
            // The new hash value.
            temp = prehash(current->pair->word2,prehash(current->pair->word1,0,table->size),table->size);
            if(temp != current->pair->hash){        // check if the hash changed after resize.
                if(prev == NULL){                   // This means it is the first element.
                    //printf("Front ");
                    table->array[i] = current->next;
                    if(table->array[temp] == NULL){ // The new spot for the current word pair is blank if true.
                       // printf("New ");
                        current->next = NULL;
                        table->array[temp] = current;
                    }else{                          // Add it to the existing linked list
                   //     printf("Old ");
                        //printf("%s %s should equal ",current->next->pair->word1,current->next->pair->word2);
                        current->next = NULL;
                        addCollisionChainQ(current, &(table->array[temp]));
                     //   printf("%s %s ",table->array[temp]->next->pair->word1,table->array[temp]->next->pair->word2 );
                    }
                }else{
                 //   printf("Middle ");
                    prev->next = current->next;
                    if(table->array[temp] == NULL){ // The new spot for the current word pair is blank if true.
               //         printf("New ");
                        current->next = NULL;
                        table->array[temp] = current;
                    }else{                          // Add it to the existing linked list
             //           printf("Old ");
                        current->next = NULL;
                        addCollisionChainQ(current, &(table->array[temp]));
                    }
                    prev = current;
                }
           //     printf("\n");
            }else{
                prev = current;
            }
            current->pair->hash = temp;
        }while((current = current->next) != NULL);
    }
}
// Frees ALL memory associated with the hash table.
void destroyHashTable(strHashTable* table){
    collisionChain* current;
    for(int i = 0; i < table->size; i++){
        if((current = table->array[i]) == NULL) continue;
        destroyCollisionChain(&current);
    }
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
    char *word1 = getNextWord(in), *word2 = getNextWord(in);
    if(word1 == NULL || word2 == NULL){
        return 0;
    }
    addHashW(word1, word2, table);
    while((word1 = strdup(word2)) != NULL && (word2 = getNextWord(in)) != NULL){
        //printf("%s %s\n",word1, word2);
        addHashW(word1, word2, table);
    }
    if(word2 == NULL){
        free(word1);
    }
    wordPair* max = getMax(table);
    printf("Max: Words(%s, %s) count : %i\n collisions in Table: %i, Table Size: %lu\n", max->word1, max->word2, max->freq,table->collisions, table->size);
    destroyHashTable(table);
    fclose(in);
    return 0;
}


