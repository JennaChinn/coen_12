/*
 *      File:           list.c
 *
 *      Description:    This file contains the functions for thie "list.h" header file
 * 
 *                      The program will create the abstract data type LIST, which consists of
 *                      a circularly double-linked list of NODEs, which store data of some generic
 *                      type. The LIST also contains a counter variable to track the amount of
 *                      NODEs and a user-provided comparison function to compare the values of the
 *                      NODEs' data. The functions can be used to create/destroy a LIST, add data
 *                      as the first/last NODE or remove a NODE, which can be the first NODE, last
 *                      NODE, or any NODE with a given value. The functions can also search the LIST
 *                      for a given value or return an array with all the data in the LIST.
 */

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

typedef struct node {
    struct node *next;
    struct node *prev;
    void *data;
} NODE; // declare NODE struct

typedef struct list {
    struct node *head;
    int count;
    int (*compare)();
} LIST; // declare LIST struct

static NODE *search(LIST *lp,void *elt) { // O(n)
    assert(lp!=NULL);
    assert(lp->compare!=NULL);
    if (lp->count==0) return NULL;
    NODE *next=lp->head->next;
    while (next!=lp->head) {
        if (next==NULL) return;
        if (lp->compare(next->data,elt)==0) return next;
        next=next->next;
    } // cycle through NODEs until elt is found
    return NULL;
} // search for elt in lp

LIST *createList(int (*compare)()) { // O(1)
    LIST *lp=malloc(sizeof(LIST));
    assert(lp!=NULL);

    lp->head=malloc(sizeof(NODE));
    assert(lp->head!=NULL);
    lp->head->next=lp->head;
    lp->head->prev=lp->head;
    lp->head->data=NULL;

    lp->count=0;
    lp->compare=compare;
    return lp;
} // create LIST

void destroyList(LIST *lp) { // O(n)
    assert(lp!=NULL);
    NODE *next=lp->head->next;
    NODE *temp;
    while (next!=lp->head) {
        temp=next->next;
        free(next);
        next=temp;
    } // free all NODEs in lp
    free(lp->head);
    free(lp);
} // free all memory associated w/lp

extern int numItems(LIST *lp) { // O(1)
    assert(lp!=NULL);
    return lp->count;
} // return total number of items

extern void addFirst(LIST *lp, void *item) { // O(1)
    assert(lp!=NULL);
    NODE *n=malloc(sizeof(NODE));
    n->data=item;
    lp->head->next->prev=n;
    n->next=lp->head->next;
    n->prev=lp->head;
    lp->head->next=n;
    lp->count++;
} // add item to beginning of lp

extern void addLast(LIST *lp, void *item) { // O(1)
    assert(lp!=NULL);
    NODE *n=malloc(sizeof(NODE));
    n->data=item;
    lp->head->prev->next=n;
    n->next=lp->head;
    n->prev=lp->head->prev;
    lp->head->prev=n;
    lp->count++;
} // add item to end of lp

extern void *removeFirst(LIST *lp) { // O(1)
    assert(lp!=NULL);
    if (lp->count==0) return NULL;
    NODE *n=lp->head->next;
    void *data=n->data;
    lp->head->next=n->next;
    n->next->prev=lp->head;
    free(n);
    lp->count--;
    return data;
} // remove first item in lp

extern void *removeLast(LIST *lp) { // O(1)
    assert(lp!=NULL);
    if (lp->count==0) return NULL;
    NODE *n=lp->head->prev;
    void *data=n->data;
    lp->head->prev=n->prev;
    n->prev->next=lp->head;
    free(n);
    lp->count--;
    return data;
} // remove last item in lp

extern void *getFirst(LIST *lp) { // O(1)
    assert(lp!=NULL);
    if (lp->count==0) return NULL;
    return lp->head->next->data;
} // return first item in lp

extern void *getLast(LIST *lp) { // O(1)
    assert(lp!=NULL);
    if (lp->count==0) return NULL;
    return lp->head->prev->data;
} // return last item in lp

extern void removeItem(LIST *lp, void *item) { // O(n)
    assert(lp!=NULL);
    NODE *n=search(lp,item); // O(n)
    if (n==NULL) return;
    n->next->prev=n->prev;
    n->prev->next=n->next;
    free(n);
    lp->count--;
} // remove item from lp

extern void *findItem(LIST *lp, void *item) { // O(n)
    assert(lp!=NULL);
    NODE *n=search(lp,item); // O(n)
    return (n!=NULL)?n->data:NULL;
} // return item if in lp

void *getItems(LIST *lp) { // O(n)
    assert(lp!=NULL);
    void **arr=malloc(sizeof(void*)*lp->count);
    NODE *next=lp->head->next;
    int amt=0;
    while (next!=lp->head) { // O(n)
        arr[amt]=next->data;
        amt++;
        next=next->next;
    } // cycle through all NODEs in lp until returned to head
    return arr;
} // return array of all items in lp