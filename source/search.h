//
//  search.h
//  VML
//
//  Created by Evan Furbeyre on 7/19/17.
//  Copyright © 2017 Evan Furbeyre. All rights reserved.
//

#ifndef search_h
#define search_h

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

void printAll();
void searchBy();
void printFile(char* path);
void displayFiles(char** words, int wc);
void deleteFile(char* path);
void editFile(char* path);
char* selectFile(char** words, int wc, int fileNum);
int numWords(char* phr);
char** initWords(char* phr, int wc);
int searchPhrase(char** words, int wc, char* path);
int searchWord(char* wrd, char* path);

#endif /* search_h */
