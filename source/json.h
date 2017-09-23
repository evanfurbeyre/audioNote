

#ifndef json_h
#define json_h

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <assert.h>

// FUNCTION PROTOTYPES
void forkExec(char* scr);
void phoneGrab();
void moveFiles(char*, char*);
void convAudio();
void sendAudio();
void convJSON();


#endif /* json_h */
