/*
 *  File:           huffman.c
 *
 *  Description:    This file contains the "huffman.c" main program
 *
 *                  The program will take two file names as input: the
 *                  file to be compressed (must be existing) and the
 *                  desired name of the outputted file. The program will
 *                  then use huffman coding to encode the characters in
 *                  order to minimize the size of the packed file.
 *              
 *                  After creating the tree containing the encoded characters,
 *                  the program will call "pack.c," which will create a
 *                  packed document according to the tree passed.The
 *                  created file can be unpacked using the "gunzip" command.
 */

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "pack.h"
#include "pqueue.h"

typedef struct node NODE;

NODE *mkNode(int data, NODE *left, NODE *right) { // O(1)
    NODE *np=malloc(sizeof(NODE));
    assert(np!=NULL);
    np->count=data;
    np->parent=NULL;
    if (left!=NULL) left->parent=np;
    if (right!=NULL) right->parent=np;
    return np;
} // creates new NODE with children left and right of count data

int getBits(NODE *np) {
    if (np==NULL) return -1;
    return 1+getBits(np->parent);
}

int compare(NODE *a, NODE *b) {
    return a->count-b->count;
}

void main(int argc, char **argv) {

    FILE *fp=fopen(argv[1],"r");
    assert(fp!=NULL);
    char letter, last;
    int s=fscanf(fp,"%c",&letter); // creates file and starts at first character

    int *freq=malloc(sizeof(int)*257); // frequency array
    assert(freq!=NULL);

    NODE **arr=malloc(sizeof(NODE)*257); // NODE array
    assert(arr!=NULL);
    int i;
    for (i=0; i<257; i++) freq[i]=0; // initialize all frequencies as 0

    int length=100;
    int count=0;
    NODE **parents=malloc(sizeof(NODE)*length);
    assert(parents!=NULL);

    while (s>=1) {
        freq[letter]++;
        last=letter;
        s=fscanf(fp,"%c",&letter);
        if (letter==EOF) freq[256]++;
    } // while next letter exists
    fclose(fp);

    PQ *pq=createQueue(&compare); // create priority queue
    for (i=0; i<257; i++) {
        if (freq[i]>0) {
            arr[i]=mkNode(freq[i],NULL,NULL);
            addEntry(pq, arr[i]); // add NODE to pq
        } // create NODE if letter is present
        else arr[i]=NULL;
    } // create NODEs for all present letters
    arr[256]=mkNode(0,NULL,NULL);
    addEntry(pq,arr[256]);

    while (numEntries(pq)>1) { // Connect all nodes to one root
        NODE *left=removeEntry(pq);
        NODE *right=removeEntry(pq);
        NODE *parent=mkNode(left->count+right->count,left,right);
        if (count>=length) {
            parents=realloc(parents,length*2*sizeof(NODE));
            length*=2;
        }
        parents[count]=parent; // add parent to parents array
        count++;
        addEntry(pq,parent); // return parent to pq
    } // While pq has unconnected nodes

    pack(argv[1],argv[2],arr);
    int bits, oct;
    for (i=0; i<257; i++) {
        if (arr[i]!=NULL) {
            bits=getBits(arr[i]);
            if (isprint(i)!=0) printf("'%c': %d x %d bits = %d bits\n",i,arr[i]->count,bits,arr[i]->count*bits); // printable character
            else {
                oct=i/64*100;
                oct+=(i%64)/8*10;
                oct+=i%8;
                printf("%d: %d x %d bits = %d bits\n",oct,arr[i]->count,bits,arr[i]->count*bits);
            } // print for unprintable character
        } // if character appears
    } // for each character in array
    destroyQueue(pq);
    for (i=0; i<257 || i<count; i++) {
        if (i<257) if (arr[i]!=NULL) free(arr[i]);
        if (i<count) free(parents[i]);
    } // free leaves and parents
    free(arr);
    free(parents);
    free(freq);
}