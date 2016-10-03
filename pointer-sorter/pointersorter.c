/**
* Systems Programming
* Asst0 - Pointer Sorter 
* Paul Vorobyev (pv149) and Vishal Rohra (vlr36) 
* Section 06
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Node {
    char * word;
    struct Node * next;
} Node;

Node * makeNode(char * word) {
    Node * node = malloc(sizeof(Node));
    node->word = word;
    node->next = NULL;
    return node;
}

/**
* Converts linked list to array of strings
* @see: Node typedef
*/
char ** listToArray(Node * head, int len) {
    Node * ptr = head;
    Node * prev = NULL;
    int ctr = len;
    char ** wordArr = malloc(sizeof(char *)*len);
    while (ctr > 0) {
        prev = ptr;
        wordArr[len-ctr] = ptr->word;
        ptr = ptr->next;
        ctr--;

        free (prev); // free Node struct. prev->word is not freed because it is reused in the array
    }

    return wordArr;
}

/*
* Preforms selection sort on an array of words using the
* strcmp() function to give strings numeric value. Runs in
* O(n^2) time.
*/
char ** sortArray(char ** words, int arrLen) { 
    int i, j;
    
    for (i = 0; i <= arrLen; i++) {
        
        int minIdx = i; 
        for (j = i+1; j <= arrLen; j++) {
           int cmp = strcmp(words[minIdx], words[j]);
           if (cmp > 0) {
              minIdx = j; 
           }
        }

        if (minIdx != i) {
            char * placeholder = words[i];
            words[i] = words[minIdx];
            words[minIdx] = placeholder;
        }
    }

    return words;
}

void printArray(char ** wordArr, int listLength) {
    int i;
    for(i = 0; i <= listLength-1; i++) {
        printf("%s\n", wordArr[i]);
    }
}


/**
* Extracts alphabetic tokens from given string into a linked list,
* converts linked list to array, sorts the array, and prints it.
*/
void parseTokens(char * tokenString) { 
    char * tokenPtr = tokenString; // pointer for characters in individual tokens
    char * wordPtr = tokenString; // pointer for holding the place of tokens in the string
    
    Node * head = NULL; 
    int listLength = 0;
    while (*wordPtr != '\0') {
   
        while (isalpha(*tokenPtr)) {
            tokenPtr++;
        }

        int strLen = tokenPtr - wordPtr;
        if (strLen != 0) { 
            char * word = (char *) malloc(strLen+1);
            memcpy(word, wordPtr, strLen);
            word[strLen] = '\0';
            Node * node = makeNode(word);
            listLength++;
            node->next = head;
            head = node;
        }

        if (*tokenPtr == '\0') {
            break;
        }
        
        wordPtr = tokenPtr;
        wordPtr++;
        tokenPtr++;
    }
    
    char ** wordArr = sortArray(listToArray(head, listLength), listLength-1);
    printArray(wordArr, listLength);
}

int main(int argc, char ** argv) {
    if (argc != 2) {
        printf("Incorrect number of arguments supplied.\n"); 
        printf("Usage: \"./pointersorter [alphabetic-tokens]\"\n");
        return 1; 
    } else {
        parseTokens(argv[1]);
    }

    return 0;
}

