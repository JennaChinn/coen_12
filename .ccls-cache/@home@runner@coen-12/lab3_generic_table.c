/*
 * File:        table.c
 *
 * Description: This file contains the functions for the "set.h" header file
 *
 *              The program will create the abstract data type SET, which
 *              contains a hash table containing some generic data type as well
 *              as the maximum length of that array and current amount of
 *              elements. The functions can then be used to add/remove elements
 *              to/from the array, get the amount of elements currently in the
 *              array, check if an element is in the array, and delete the
 *              entire SET.
 *
 *              The hash function is arranged with linear probing and using a
 *              flag array to keep track of each index's status. For this flag
 *              array, -1 indicates a deleted item, 0 indicates an unused index,
 *              and 1 indicates a filled index.
 */


#include <assert.h>
#include <stdlib.h>
#include <string.h>

typedef struct set {
  void ** data;
  int * flag;
  int length;
  int count;
  int (*compare)();
  unsigned (*hash)();
} SET; // create SET struct

static int search(SET *sp, void *elt) { // O(n)
  assert(sp!=NULL);
  int i=0;
  int loc=sp->hash(elt)%sp->length; // loc = home index
  while (sp->flag[(loc+i)%sp->length]!=0 && i<sp->length) {
    if (sp->flag[(loc+i)%sp->length]==1) if (sp->compare(sp->data[(loc+i)%sp->length],elt)==0) return (loc+i)%sp->length; // if data[loc] is filled and data[loc]==elt return index
    i++;
  } // while location is empty and i< length
  return -1;
} // search for element

SET *createSet(int maxElts, int (*compare)(), unsigned (*hash)()) { // O(m)
  SET *sp;
  sp=malloc(sizeof(SET));
  assert(sp!=NULL); // create SET sp

  sp->data = malloc(sizeof(void*)*maxElts);
  assert(sp->data!=NULL); // create data

  sp->flag=malloc(sizeof(int)*maxElts);
  assert(sp->flag!=NULL);
  int i;
  for (i=0; i<maxElts; i++) sp->flag[i]=0; // create and populate flag

  sp->compare=compare;
  assert(sp->compare!=NULL);
  sp->hash=hash;
  assert(sp->hash!=NULL); // assign compare and hash

  sp->count=0;
  sp->length=maxElts; // assign count and length
  return sp;
} // create new SET

void destroySet(SET *sp) { // O(1)
  assert(sp!=NULL);
  free(sp->data);
  free(sp->flag);
  free(sp); // free sp and its arrays
} // destroy SET sp

int numElements(SET *sp) { // O(1)
  assert(sp!=NULL);
  return sp->count;
} // return count

void addElement(SET *sp, void *elt) { // O(n)
  assert(sp!=NULL);
  if (sp->count==sp->length) return; // ensure sp is not full
  int loc=sp->hash(elt)%sp->length; // find home index
  int i=0;
  if (search(sp,elt)!=-1) return; // ensure elt is not in sp
  while (sp->flag[loc+i]==1) i++;
  sp->data[loc+i]=elt;
  sp->flag[loc+i]=1;
  sp->count++; // insert elt into data
} // insert element into sp

void removeElement(SET *sp, void *elt) { // O(n)
  assert(sp!=NULL);
  int loc=search(sp,elt); // get index
  if (loc==-1) return; // return if not in sp
  sp->data[loc]=NULL;
  sp->flag[loc]=-1;
  sp->count--; // remove elt from sp
} // remove element from sp

void *findElement(SET *sp, void *elt) { // O(n)
  assert(sp!=NULL);
  int loc=search(sp,elt); // get index
  return (loc==-1)?NULL:sp->data[loc]; // return loc if elt exists
} // find elt in sp

void **getElements(SET *sp) { // O(m)
  void **arr;
  assert(sp!=NULL);
  arr=malloc(sizeof(void*)*sp->count); // create array of length count
  int i;
  int num=0;
  for (i=0; i<sp->length; i++) {
    if (sp->flag[i]==1) {
      arr[num]=sp->data[i]; // add element to arr
      num++;
    } // if element exists
  } // for each element in sp
  return arr;
} // return array of existing elements in sp