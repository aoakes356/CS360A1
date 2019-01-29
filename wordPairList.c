#include "strHash.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

wordPairList* newWordPairList(){ // Initialize a new wordPairList.
    wordPairList* new = malloc(sizeof(wordPairList));
    assert(new != NULL);
    new->size = 64;
    new->used = 0;
    new->array = malloc(sizeof(wordPair*)*new->size);
    for(int i = 0; i < new->size; i++){
        new->array[i] = NULL;
    }
    assert(new->array != NULL);
    return new;
}

void pushWordPair(wordPair* wp, wordPairList* list){
    list->array[list->used++] = wp;
    if(list->used >= list->size-1){
        list->size *= 2;
        list->array = realloc(list->array,list->size*sizeof(wordPair*));
        assert(list->array != NULL);
        for(int i = list->used; i < list->size; i++){
            list->array[i] = NULL;
        }
    }
}

// Remove the wordpair, Does not take responsibility for freeing wp. 
int removeWordPair(wordPair* wp, wordPairList* list){
    int i;
    for(i = 0; list->array[i] != wp && i < list->used; i++);
    if(i == list->used){
        return 0;
    }
    return removeWordPairI(i,list);
}

// Remove the wordPair at the given index.
int removeWordPairI(int index, wordPairList* list){
    if(index >= list->used){
        return 0;
    }
    list->array[index] = list->array[list->used-1];
    list->array[list->used-1] = NULL;
    list->used--;
    return 1;

}

void destroyWordPairList(wordPairList** list){
    free((*list)->array);
    free(*list);
    *list = NULL;
}

int comparator(const void* w1, const void* w2){
    return (*(wordPair**)w2)->freq - (*(wordPair**)w1)->freq;
}

void sortWordPairList(wordPairList* list){
    qsort(list->array,list->used-1,sizeof(wordPair*),&comparator);
}

// Prints count elements from the top of the array.
void printTop(int count, wordPairList* list){
    for(int i = 0; i < list->used && i < count; i++){
        printf("%10lu %s\n",list->array[i]->freq, list->array[i]->words);
    }
    
}

