/*
 * File:        unsorted.c
 *
 * Description: This file contains the functions for the "set.h" header file
 *
 *              The program will create the abstract data type SET, which
 *              contains an unsorted array of strings as well as the maximum
 *              length of that array and current amount of elements. The
 *              functions can then be used to add/remove elements to/from the
 *              array, get the amount of elements currently in the array, check
 *              if an element is in the array, and delete the entire SET.
 */
#include <assert.h>
#include <stdlib.h>
#include <string.h>

typedef struct set {
    char ** data;
    int length;
    int count;
} SET; //declare SET structure

static int search(SET *sp, char *elt) { // O(n)
    assert(sp!=NULL);
    int i;
    for (i=0; i<sp->count; i++) if (strcmp(elt,sp->data[i])==0) return i; // return i if data[i] is same as elt
    return -1; // -1 if no match found
}

SET *createSet(int maxElts) { // O(1)
    SET *sp;
    sp = malloc(sizeof(SET));
    assert(sp!=NULL); // ensure malloc was successful
    sp->data = malloc(sizeof(char*)*maxElts);
    assert(sp->data!=NULL); // ensure malloc was successful
    sp->length=maxElts;
    sp->count=0;
    return sp;
}

void destroySet(SET *sp) { // O(n)
    assert(sp!=NULL);
    int i;
    for (i=0; i<sp->count; i++) free(sp->data[i]); // free all elements in data
    free(sp->data);
    free(sp);
}

int numElements(SET *sp) { // O(1)
    assert(sp!=NULL);
    return sp->count;
}

void addElement(SET *sp, char *elt) { // O(1)
    assert(sp!=NULL);
    if (sp->count>=sp->length) return; // return if there is no space
    if (search(sp,elt)!=-1) return; // return if elt already exists
    sp->data[sp->count]=strdup(elt); // add elt to end
    sp->count++;
}

void removeElement(SET *sp, char *elt) { // O(1)
    assert(sp!=NULL);
    int loc=search(sp,elt);
    if (loc==-1) return; // return if elt does not exist
    free(sp->data[loc]);
    sp->data[loc]=sp->data[sp->count-1];
    sp->data[sp->count-1]=NULL; // set former location of last element to NULL
    sp->count--;
}

char *findElement(SET *sp, char *elt) { // O(1)
    assert(sp!=NULL);
    int loc = search(sp,elt);
    return loc==-1?NULL:sp->data[loc]; // return NULL if elt does not exist, else return the string that matches elt
}

char **getElements(SET *sp) { // O(1)
    assert(sp!=NULL);    
    char **arr = malloc(sizeof(char*) * sp->count); // array of size count
    memcpy(arr,sp->data,sizeof(char*)*sp->count); // all elements in data into arr
    return arr;
}