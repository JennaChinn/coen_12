#include <assert.h>
#include <stdlib.h>
#include <string.h>
#define LENGTH 100

typedef struct node {
    int start;
    int count;
    void **data; // null pointer array
    struct node *next;
    struct node *prev;
    int length;
} NODE; // NODE struct

typedef struct list {
    NODE *head;
    int count; // total items across NODEs
} LIST; // LIST struct

static NODE *createNode() { // O(n)
    NODE *np=malloc(sizeof(NODE));
    np->length=LENGTH;

    np->data=malloc(sizeof(void*)*np->length); // array of void*
    int i;
    for (i=0; i<np->length; i++) np->data[i]=NULL;
    np->count=0;
    np->start=np->length/5; // start near beginning, doesn't really matter though
    return np;
} // creates new NODE

static NODE *addFront(LIST *lp) { // O(n)
    assert(lp!=NULL);
    NODE *np=createNode(); // O(n)

    np->next=lp->head->next;
    np->next->prev=np;

    np->prev=lp->head;
    lp->head->next=np;
    return np;
} // add new NODE to front of lp

static NODE *addBack(LIST *lp) { // O(n)
    assert(lp!=NULL);
    NODE *np=createNode(); // O(n)

    np->prev=lp->head->prev;
    np->prev->next=np;

    np->next=lp->head;
    lp->head->prev=np;
    return np;
} // add new NODE to back of lp

static NODE *getIndex(LIST *lp, int *index) { // O(m)
    assert(lp!=NULL);
    NODE *np=lp->head->next;
    while (*index>=np->count) { // O(m)
        *index-=np->count;
        np=np->next;
        if (np==lp->head) return NULL;
    } // iterate through each NODE in lp
    return np;
} // get NODE & index at index of lp

LIST *createList(void) { // O(1)
    LIST *lp=malloc(sizeof(LIST));
    lp->count=0;

    lp->head=malloc(sizeof(NODE)); // dummy NODE
    lp->head->next=lp->head;
    lp->head->prev=lp->head;
    lp->head->data=NULL;
    return lp;
} // create new LIST

void destroyList(LIST *lp) { // O(m)
    assert(lp!=NULL);
    NODE *np=lp->head->next;
    NODE *temp;
    while (np!=lp->head) { // O(m); m=amt. NODEs
        free(np->data);
        temp=np->next;
        free(np);
        np=temp;
    } // free data associated with all NODEs in lp
    free(lp->head);
    free(lp);
} // free all memory associated with lp

int numItems(LIST *lp) { // O(1)
    assert(lp!=NULL);
    return lp->count;
} // return total items across all NODEs in lp

void addFirst(LIST *lp,void *item) { // O(n); O(1) expected
    assert(lp!=NULL);
    NODE *np=lp->head->next;
    if (lp->count==0) np=addFront(lp); // O(n)
    else if (np->count==np->length) np=addFront(lp); // O(n)
    np->start=(np->length+np->start-1)%np->length; // index behind start
    np->data[np->start]=item;
    np->count++;
    lp->count++;
} // add item as 1st item in lp

void addLast(LIST *lp, void *item) { // O(n); O(1) expected
    assert(lp!=NULL);
    NODE *np=lp->head->prev;
    if (lp->count==0) np=addBack(lp); // O(n)
    else if (np->count==np->length) np=addBack(lp); // O(n)
    int loc=(np->start+np->count)%np->length; // 1st unfilled index
    np->data[loc]=item;
    np->count++;
    lp->count++;
} // add item as last item in lp

void *removeFirst(LIST *lp) { // O(1)
    assert(lp!=NULL);
    if (lp->count==0) return NULL;
    NODE *np=lp->head->next;
    void *data=np->data[np->start]; // store removed data
    np->data[np->start]=NULL;
    np->start=(np->start+1)%np->length; // move np->start up 1
    np->count--;
    lp->count--;
    if (np->count==0) { // O(1)
        lp->head->next=np->next;
        np->next->prev=lp->head;
        free(np->data);
        free(np);
    } // remove np if it's empty
    return data;
} // remove 1st item in lp

void *removeLast(LIST *lp) { // O(1)
    assert(lp!=NULL);
    if (lp->count==0) return NULL;
    NODE *np=lp->head->prev;
    int loc=(np->start+np->count-1+np->length)%np->length; // last filled index
    void *data=np->data[loc]; // store removed data
    np->data[loc]=NULL;
    np->count--;
    lp->count--;
    if (np->count==0) { // O(1)
        lp->head->prev=np->prev;
        np->prev->next=lp->head;
        free(np->data);
        free(np);
    } // remove np if it's empty
    return data;
} // remove last item in lp

void *getFirst(LIST *lp) { // O(1)
    assert(lp!=NULL);
    NODE *np=lp->head->next;
    return np->data[np->start];
} // return first item in lp

void *getLast(LIST *lp) { // O(1)
    assert(lp!=NULL);
    NODE *np=lp->head->prev;
    return np->data[(np->start+np->count-1+np->length)%np->length];
} // return last item in lp

void *getItem(LIST *lp, int index) { // O(m)
    assert(lp!=NULL);
    NODE *np=getIndex(lp,&index); // O(m)
    void *x= (np==NULL)?NULL:np->data[(index+np->start)%np->length];
    return x;
} // get item at index of lp

void setItem(LIST *lp, int index, void *item) { // O(m)
    assert(lp!=NULL);
    NODE *np=getIndex(lp,&index); // O(m)
    if (np==NULL) return;
    int ind=(np->start+index)%np->length; // Index in np->data
    np->data[ind]=item;
} // set item at index of lp