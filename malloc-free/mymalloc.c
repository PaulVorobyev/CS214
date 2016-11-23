#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct Node {
    int start;
    int size;
    int filled;
    struct Node * next;
} Node;

static char memory[5000];
Node head = {0, 0, 0, NULL};

void * mymalloc(size_t size) {
    Node * ptr = &head;
    Node * prev = NULL;
    
    if(head.size == 0) { // head is uninitialized
        ptr->start = 0;
        ptr->size = (int)size;
        ptr->filled = 1;
        return (void *)memory;
    }
    
    // otherwise, traverse LL
    int currOffset = 0;
    while(ptr != NULL) {
        if (ptr->filled == 0 && ptr->size >= size) {
            ptr->start = currOffset; 
            ptr->size = (int)size;
            ptr->filled = 1;
            return (void *)memory+currOffset;
        } else {
            currOffset += ptr->size;
            prev = ptr;
            ptr = ptr->next;
        }
    }

    // no candidate found, create new Node at the end
    Node end = {currOffset, (int)size, 1, NULL};
    prev->next = &end;
    return (void *)memory+(currOffset);
}

void free(void * ptr) {
    int relativeAddr = ptr-(void *)memory;
    int currOffset = 0;
    
    Node * ptr = &head;
    Node * prev = NULL;

    while(ptr != NULL) {
        if(offset == counter) {
            ptr->filled = 0;
        } else {
            prev = ptr;
            counter += prev;
            ptr = ptr->next;
        }
    }
}

int main(int argc, char ** argv) {
    char * thing = (char *)mymalloc(3);
    thing[0] = 'a';
    thing[1] = 'b';
    thing[2] = '\0';
    
    char * thing2 = (char *)mymalloc(3);
    thing2[0] = 'c';
    thing2[1] = 'd';
    thing2[2] = '\0';

    char * thing3 = (char *)mymalloc(3);
    thing3[0] = 'e';
    thing3[1] = 'f';
    thing3[2] = '\0';
    printf("%s %s %s", thing, thing2, thing3);
    
    free(thing3);
    return 0;
}
