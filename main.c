#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "hw1/getWord/include/getWord.h"
#include "strHash.h"


int main(int argc, char** argv){
    // Check if it has a useable number of arguments.
    if(argc < 2){
        printf("Incorrect Usage\n wordpairs <-count> fileName1 <fileName2> <fileName3> ...\n");
        return 0;
    }

    int count = 0, fileCount = argc-1, start = argc;
    // Check if the number of elements to be printed is an argument.
    if(sscanf(argv[1],"-%d",&count) || sscanf(argv[1],"-count=%d",&count) || sscanf(argv[1],"%d",&count) ){
        if(argc < 3){
            printf("Specify at least one file path\n");
            return 1;
        }
        fileCount--;
    }

    start = argc - fileCount;

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
            fclose(in);
            continue;
        }
        addHashW(word1, word2, table);
        free(word1);
        word1 = NULL;
        // Loop through aiding each pair to the hash table.
        while((word1 = word2) != NULL && (word2 = getNextWord(in)) != NULL){
            addHashW(word1, word2, table);
            free(word1);
            word1 = NULL;
        }
        if(word2 == NULL && i == fileCount -1){
            free(word1);
            word1 = NULL;
        }
        fclose(in);
    }

    // Shouldn't need both of these but i'm paranoid.

    if(word1 != NULL){
        free(word1);
        word1 = NULL;
    }
    if(word2 != NULL){
        free(word2);
        word2 = NULL;
    }

    if(count > 0){                              // Check if the amount to be printed has been specified.
        printTopH(count,table);
    }else{
        printTopH(table->keys->used-1, table);  // If not print them all.
    }

    //printf("Collisions: %i, Table Size: %lu\n",table->collisions, table->size); // Uncomment to see collisions and table size.
    destroyHashTable(table);
    return 0;
}

