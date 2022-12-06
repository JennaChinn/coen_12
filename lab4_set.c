/*
 *      File:           set.c
 *
 *      Description:    This file contains the functions for the "set.h" header file
 * 
 *                      The program will create the abstract data type SET, which contains a hash
 *                      table consisting of LIST ADTs, which contain some generic data type. The
 *                      SET also contains variables to record the total amount of elements, the
 *                      amount of LISTs, and user-provided functions compare (compares the SET's
 *                      data) and hash (maps any provided value to some location). The functions
 *                      can create/destroy a SET, return the amount of elements in the SET, add/
 *                      remove elements from the SET, search for any given value in the SET's LISTs,
 *                      and return an array with every element in the SET.
 */

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

typedef struct set {
    LIST ** data;
    int count;
    int length;
    int (*compare)();
    unsigned (*hash)();
} SET; // Declare SET struct

SET *createSet(int maxElts, int (*compare)(), unsigned (*hash)()) { // O(n)
    SET *sp;
    sp=malloc(sizeof(SET));
    assert(sp!=NULL);

    sp->data=malloc(sizeof(LIST*)*maxElts/20);
    assert(sp->data!=NULL);
    int i;
    for (i=0;i<maxElts/20;i++) sp->data[i]=createList(compare); // Create LISTs

    sp->compare=compare;
    sp->hash=hash;

    sp->count=0;
    sp->length=maxElts/20;
    return sp;
} // Creates new SET

void destroySet(SET *sp) { // O(n)
    assert(sp!=NULL);
    int i;
    for (i=0; i<sp->length; i++) destroyList(sp->data[i]);
    free(sp);
} // frees SET

int numElements(SET *sp) { // O(1)
    assert(sp!=NULL);
    return sp->count;
} // returns total number of elements

void addElement(SET *sp, void *elt) { // O(n)
    assert(sp!=NULL);
    int loc=sp->hash(elt)%sp->length;
    void *item=findItem(sp->data[loc],elt); // O(n)
    if (item!=NULL) return;
    addFirst(sp->data[loc],elt);
    sp->count++;
} // adds elt to beginning of mapped LIST

void removeElement(SET *sp,void *elt) { // O(n)
    assert(sp!=NULL);
    int loc=sp->hash(elt)%sp->length;
    void *item=findItem(sp->data[loc],elt); // O(n)
    if (item==NULL) return;
    removeItem(sp->data[loc],elt);
    sp->count--;
} // removes elt from mapped LIST

void *findElement(SET *sp,void *elt) { // O(n)
    assert(sp!=NULL&&elt!=NULL);
    int loc=(sp->hash(elt))%(sp->length);
    void *item=findItem(sp->data[loc],elt); // O(n)
    return item;
} // searches fro elt in mapped LIST

void *getElements(SET *sp) { // O(n)
    void **arr;
    void **subArr;
    assert(sp!=NULL);
    arr=malloc(sizeof(void**)*sp->count);
    int i,len;
    int tot=0;
    for (i=0; i<sp->length; i++) {
        subArr=getItems(sp->data[i]);
        len=numItems(sp->data[i]);
        memcpy(arr+tot,subArr,sizeof(void*)*len);
        tot+=len;
    } // O(n)
    return arr;
} // returns array of all elements in sp