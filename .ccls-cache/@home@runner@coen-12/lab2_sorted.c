/*
 * File:       
 *
 * Description: This file contains the functions for the "set.h" header file
 *
 *              The program will create the abstract data type SET, which
 *              contains a sorted array of strings as well as the maximum
 *              length of that array and current amount of elements. The
 *              functions can then be used to add/remove elements to/from the
 *              array, get the amount of elements currently in the array,
 *              check if an element is in the array, and delete the entire SET.
 */

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct set {
  char **data;
  int length;
  int count;
} SET; // declare SET structure

static int search(SET *sp, char *elt, bool *found) { // O(logn)
  assert(sp!=NULL); // ensure sp is not null
  int hi=sp->count-1;
  if (hi==-1) return 0; // return 0 if no elements
  int lo=0; // set high/low values
  int mid,comp;
  while(lo<hi) {
    mid=(lo+hi)/2; // set mid as avg. of lo & hi
    comp=strcmp(elt,sp->data[mid]); // compare elt with string at index mid
    if (comp==0) {
      *found=true;
      return mid;
    } // return mid if the string at mid is equivalent to elt
    if (comp>0) lo=mid+1;
    else hi=mid-1; // change parameters to fit comparison
  }
  if (lo<sp->count) {
    if (!strcmp(sp->data[lo],elt)) {
	*found=true;
    }
  } // ensure elt is not equivalent to string at index lo
  if (hi>0) {
    if (!strcmp(sp->data[hi],elt)) {
      *found=true;
      return hi;
    }
  } // ensure elt is not equivalent to string at index hi
  if (strcmp(elt,sp->data[lo])>0) return lo+1; // return lo+1 if elt is higher than lo
  return lo;
}

SET *createSet(int maxElts) { // O(1)
  SET *sp;
  sp = malloc(sizeof(SET));
  assert(sp!=NULL); //ensure malloc was successful
  sp->data = malloc(sizeof(char*)*maxElts);
  assert(sp->data!=NULL); //ensure malloc was successful
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

void addElement(SET *sp, char *elt) { // O(n)
  assert(sp!=NULL);
  if (sp->count>=sp->length) return;
  bool f=false;
  int loc;
  loc=search(sp,elt,&f); // set loc to result of search
  if (f) return; // return if elt already exists
  int i;
  for (i=sp->count;i>loc; i--) {
    sp->data[i]=strdup(sp->data[i-1]);
    free(sp->data[i-1]);
  } // move each string above loc up one
  sp->data[loc]=strdup(elt); // copy elt to index loc
  sp->count++;
}

void removeElement(SET *sp, char *elt) { // O(n)
  assert(sp!=NULL);
  bool f=false;
  int loc=search(sp,elt,&f);
  if (!f) return;
  free(sp->data[loc]); // free data at loc
  int i;
  //sp->data[loc]=malloc(sizeof(char*));
  for (i=loc; i<sp->count-1; i++) sp->data[i]=sp->data[i+1]; // move each element above loc down one
  sp->data[sp->count-1]=NULL;
  sp->count--;
}

char *findElement(SET *sp, char *elt) { // O(1)
  assert(sp!=NULL);
  bool f=false;
  int loc=search(sp,elt,&f);
  return f?sp->data[loc]:NULL; // return string that matches elt if found, else return NULL
}

char **getElements(SET *sp) { // O(1)
  assert(sp!=NULL);
  char **arr = malloc(sizeof(char*)*sp->count); // array of size count
  memcpy(arr,sp->data,sizeof(char*)*sp->count); // all elements in data into arr
  return arr;
}