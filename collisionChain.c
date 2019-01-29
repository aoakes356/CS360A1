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
    destroyWordPair(&((*head)->pair));
    free(*head);
    *head = NULL;
}

void destroyCollisionChainWP(collisionChain** head){ // Destroy the collisionchain without freeing the word pairs.
    if(*head == NULL) return;
    destroyCollisionChain(&((*head)->next));
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

// Remove the collisionChain containing the given wordpair without destroying the wordpair.
int removeWordPairCC(collisionChain** head, wordPair* deleteThis){
    collisionChain* current = *head, *previous = NULL;
    if(*head == NULL){
        return 0;
    }
    do{
        if(deleteThis == current->pair){
            if(previous == NULL){
                *head = (*head)->next;
                current->next = NULL;
                destroyCollisionChainWP(&current);
                return 1;
            }
            previous->next = current->next;
            current->next = NULL;
            destroyCollisionChainWP(&current);
            return 1;
        }
        previous = current;
    }while((current = current->next) != NULL);
    return 0;
}

void printCollisionChain(collisionChain* head){
    if(head == NULL){
        printf("NULL \n");
        return;
    }
    do{
        printf("%s --> ",head->pair->words);
    }while((head = head->next) != NULL);
    printf("==-?>1;\n");
}



