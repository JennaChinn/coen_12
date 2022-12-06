#include <stdio.h>
#define MAX_WORD_LENGTH 30
void main(int argc, char *argv[]) {
	FILE *fp = fopen(*(argv+1),"r"); // open file from input
	char word[MAX_WORD_LENGTH];
	int total=0;
	int scanned=1;
	fscanf(fp,"%s",word); // get 1st word
	while (scanned == 1) {
		total++;
		scanned = fscanf(fp,"%s",word); // get next word
	} // while a word was scanned
	printf("%d total words\n",total);
	fclose(fp); // close file
}