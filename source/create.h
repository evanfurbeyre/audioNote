
#ifndef create_h
#define create_h

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <assert.h>


// FUNCTION PROTOTYPES
void readParseCreate();
void parse(char*);
FILE* command(char* tkn, FILE* fp, char* dateId);
int keywordErrors(char* txt);
FILE* page(char* tkn, FILE* fp, char* dateId);
void punct(char* tkn, FILE* fp);
void tagify(char* tkn, FILE* fp);
void endpage(char* tkn, FILE* fp, char* dateId);


#endif /* create_h */
