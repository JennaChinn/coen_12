/*
 * File:        table.c
 *
 * Description: This file contains the functions for the "set.h" header file
 *
 *              The program will create the abstract data type SET, which
 *              contains a hash table containing strings as well as the maximum
 *              length of that array and current amount of elements. The
 *              functions can then be used to add/remove elements to/from the
 *              array, get the amount of elements currently in the array, check
 *              if an element is in the array, and delete the entire SET.
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
  char ** data;
  int * flag;
  int length;
  int count;
} SET; // declare SET structure

static unsigned strhash(char *str) {
  unsigned hash=0;
  while (*str!='\0') hash=31*hash+*str++;
  return hash;
} // get home index of str

static int search(SET *sp, char *elt) {
  assert(sp!=NULL);
  int i=0;
  int loc=strhash(elt)%sp->length; // loc = home index
  while (sp->flag[loc+i]!=0 && i<sp->length) {
    if (sp->flag[(loc+i)%sp->length]==1) if (strcmp(sp->data[(loc+i)%sp->length],elt)==0) return (loc+i)%sp->length; // return if elt is found
    i++;
  } // while flag[loc+i] is not empty
  return -1; // -1 if not found
} // search for elt in set

SET *createSet(int maxElts) {
  SET *sp;
  sp = malloc(sizeof(SET));
  assert(sp!=NULL); // SET pointer

  sp->data = malloc(sizeof(char*)*maxElts);
  assert(sp->data!=NULL); // data array

  sp->flag = malloc(sizeof(int)*maxElts);
  assert(sp->flag!=NULL);
  int i;
  for (i=0; i<maxElts; i++) sp->flag[i]=0; // flag array

  sp->length=maxElts;
  sp->count=0;
  return sp; // return SET pointer
} // create SET of size maxElts

void destroySet(SET *sp) {
  assert(sp!=NULL);
  int i;
  for (i=0; i<sp->length; i++) if (sp->flag[i]==1) free(sp->data[i]); // free all existing strings in data
  free(sp->data);
  free(sp->flag);
  free(sp); // free sp and arrays
} // free sp and all data

int numElements(SET *sp) {
  assert(sp!=NULL);
  return sp->count;
} // get number of elements in sp

void addElement(SET *sp, char *elt) {
  assert(sp!=NULL);
  if (sp->count == sp->length) return;
  int loc=strhash(elt)%sp->length; // get home index
  int i=0;
  if (search(sp,elt)!=-1) return;
  while (sp->flag[(loc+i)%sp->length]==1) {
    if (strcmp(sp->data[loc+i],elt)==0) return; // return if elt already exists
    i++;
  } // while data[loc+i] is filled
  sp->data[loc+i]=strdup(elt); // copy elt into data
  sp->flag[loc+i]=1; // mark data[loc+i] as filled
  sp->count++;
} // add elt to sp if not already in sp

void removeElement(SET *sp, char *elt) {
  assert(sp!=NULL);
  int loc=search(sp,elt); // find elt in sp
  if (loc==-1) return; // return if elt not in sp
  free(sp->data[loc]);
  sp->flag[loc]=-1; // mark loc as removed
  sp->count--;
} // remove elt from sp if it exists

char *findElement(SET *sp, char *elt) {
  assert(sp!=NULL);
  int loc=search(sp,elt); // get index of elt
  return (loc==-1)?NULL:sp->data[loc]; // return NULL if elt not in sp, else string matching elt
} // find elt in sp

char **getElements(SET *sp) {
  char **arr;
  assert(sp!=NULL);
  arr = malloc(sizeof(char*)*sp->count); // create array of size count
  int i;
  int num=0;
  for (i=0; i<sp->length; i++) {
    if (sp->flag[i]==1) {
      arr[num]=sp->data[i]; // add data[i] to arr
      num++;
    } // if data[i] has a value
  } // for each element in data
  return arr;
} // return array of elements in data