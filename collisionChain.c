#include "strHash.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
/* construct a new collisionChain structure */
collisionChain* newCollisionChainW(char* w1, char* w2){
    return newCollisionChainP(newWordPair(w1,w1));
}

collisionChain* newCollisionChainP(wordPair* wp){
    collisionChain* newChain = (collisionChain*)malloc(sizeof(collisionChain));
    assert(newChain != NULL);
    newChain->pair = wp;
    newChain->next = NULL;
    return newChain;
}


/* Frees ALL memory associated with the collision chain, sets the pointer to null. */
void destroyCollisionChain(collisionChain** head){
    if(*head == NULL) return;
    destroyCollisionChain(&((*head)->next));
    destroyWordPair((*head)->pair);
    free(*head);
    *head = NULL;
}

/* add to the front of a collision Chain */
void addCollisionChainW(char* w1, char* w2, collisionChain** head){
    collisionChain* newHead = newCollisionChainW(w1,w2);
    addCollisionChainQ(newHead,head);
}

void addCollisionChainP(wordPair* wp, collisionChain** head){
    collisionChain* newHead = newCollisionChainP(wp);
    addCollisionChainQ(newHead, head);
}

void addCollisionChainQ(collisionChain* new, collisionChain** head){
    new->next = *head;
    *head = new;
}

void printCollisionChain(collisionChain* head){
    if(head == NULL) return;
    do{
        printf("%s %s --> ",head->pair->word1,head->pair->word2);
    }while((head = head->next) != NULL);
    printf("==-?>1;\n");
}



