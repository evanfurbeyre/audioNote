//
//  search.c
//  VML
//
//  Created by Evan Furbeyre on 7/19/17.
//  Copyright © 2017 Evan Furbeyre. All rights reserved.
//

#include "search.h"

/***********************************************************
 printAll()
 
 
 ***********************************************************/
void printAll(){
    DIR* d = opendir("./pages");
    struct dirent *dir;
    char path[64];
    
    if (d){
        // If directory opened, read through each file beginning with page
        while ((dir = readdir(d)) != NULL ){
            if (strncmp(dir->d_name, "page", 4) == 0){
                // Get the path to that file, and then search it
                sprintf(path, "pages/%s", dir->d_name);
                // Search the file at the path for the phrase, print if found
                printFile(path);
            }
        }
    }
}

/***********************************************************
 searchBy()
 

 ***********************************************************/
void searchBy(){
    // Get phrase from user to filter pages by
    printf("Enter a word(s) to search by: ");
    char* buf = NULL;
    size_t sb;
    getline(&buf, &sb, stdin);
    buf[strcspn(buf, "\n")] = ' '; // Turn newline into a space
    
    int wc = numWords(buf);                 // Count number of search terms
    char** words = initWords(buf, wc);      // Dynamically allocate array of strings of each word in the search phrase
    
    displayFiles(words, wc);
    printf("\nEnter a file number to select, or 0 to exit\n");
    int sel;
    char* path = NULL;
    while(1){       // Make sure user selected a valid file
        sel = getc(stdin) - 48;
        getc(stdin);    // remove extra \n
        if (sel == 0){
            break;
        }
        path = selectFile(words, wc, sel);    // selectFile returns the path to the file with corresponding sel number
        if (path == NULL){
            printf("File not found, enter a file number\n");
            free(path);
        }else{
            break;
        }
    }
    
    if (sel != 0 && path != NULL){
        // Operations to perform on file
        printf("\nFile Operations\n");
        printf("    1) Delete\n");
        printf("    2) Edit\n");
        printf("    3) Exit\n");
        
        int option2 = getc(stdin) - 48;
        getc(stdin);  // remove extra \n
        
        switch(option2){
            case 1: {   // DELETE FILE
                deleteFile(path);       // Asks for confirmation, then deletes
                break;
            }
            case 2: {   // EDIT FILE
                editFile(path);         // Opens vim, will only work in terminal
                break;
            }
            case 3: {   // EXIT MENU
                printf("Exiting...\n");
                break;
            }
            default: {
                printf("Please enter a valid option\n");
            }
        }
    }
}



/***********************************************************
 printFile()
 
 ***********************************************************/
void printFile(char* path){
    if (path == NULL){
        printf("File not found");
    }else{
        FILE* fp = fopen(path, "r");
        char* rbuf = NULL;
        size_t sb = 0;
        ssize_t nr;
        while(( nr = getline(&rbuf, &sb, fp)) != -1 ){
            printf("%s", rbuf);
        }
        fclose(fp);
    }
}

/***********************************************************
 displayFiles()
 
 Display the file printouts with numbered indices that contain all the strings in words array,
 ***********************************************************/
void displayFiles(char** words, int wc){
    DIR* d = opendir("./pages");
    struct dirent *dir;
    char path[64];
    
    if (d){
        // If directory opened, read through each file beginning with page
        int i = 1;      // File count variable
        while ((dir = readdir(d)) != NULL ){
            if (strncmp(dir->d_name, "page", 4) == 0){
                // Get the path to that file, and then search it
                sprintf(path, "pages/%s", dir->d_name);
                // Search the file at the path for the phrase, print if found
                if (searchPhrase(words, wc, path)){
                    printf("\n(%i)", i);
                    printFile(path);
                    i++;
                }
            }
        }
    }
}

/***********************************************************
 selectFile()
 
 Will return the path of the corresponding file to fileNum, the index from the displayFiles call.
 ***********************************************************/
char* selectFile(char** words, int wc, int fileNum){
    // Dynamically Allocate path to be returned
    char* path = (char*)malloc(sizeof(char)*64);
    
    DIR* d = opendir("./pages");
    struct dirent *dir;
    
    if (d){
        // If directory opened, read through each file beginning with page
        int i = 1;      // File count variable
        while ((dir = readdir(d)) != NULL ){
            if (strncmp(dir->d_name, "page", 4) == 0){
                // Get the path to that file, and then search it
                sprintf(path, "pages/%s", dir->d_name);
                // Search the file at the path for the phrase, print if found
                if (searchPhrase(words, wc, path)){
                    if (i == fileNum){
                        return path;
                    }
                    i++;
                }
            }
        }
    }
    return NULL;
}

/***********************************************************
 deleteFile()
 
 Confirms and deletes the file at path
 ***********************************************************/
void deleteFile(char* path){
    printf("\nDelete this note? (y/n)\n");
    printFile(path);
    char confDel = getc(stdin);
    getc(stdin);  // remove extra \n
    if (strncmp(&confDel, "y", 1) == 0){
        if(remove(path) == 0){
            printf("Deleted File\n");
        }else{
            printf("Could not delete file\n");
        }
    }
}

/***********************************************************
 editFile()
 
 Opens vim on the file at path
 ***********************************************************/
void editFile(char* path){
    pid_t sPid = -9;        // Holds result of fork, 0 for child, childPID for parent
    int chExStat = -9;      // Child exit status
    
    sPid = fork();              // Fork Here!
    switch(sPid){
        case -1: {       // ERROR
            perror("Fork error!\n"); fflush(stdout);
            exit(1); break;
        }
        case 0: {        // CHILD
            execlp("vi", "vi", path, NULL);
        }
        default:{        // PARENT
            sPid = waitpid(sPid, &chExStat, 0);      // Clean up child process
            if (sPid == -1) {perror("parent waitpid failure"); exit(1);}
        }
    }
}

/***********************************************************
 numWords()
 
 Count the number of words delineated by spaces in a phrase
 ***********************************************************/
int numWords(char* phr){
    // Count the number of words
    int wc = 0;
    int i;
    for (i = 0; i < strlen(phr); i++)
        if (phr[i] == ' ')
            wc++;
    
    return wc;
}

/***********************************************************
 initWords()
 
 Parses a phrase by spaces, and returns a dynamically allocated array of words
 ***********************************************************/
char** initWords(char* phr, int wc){
    // Allocate memory, a string for each word
    char** words = malloc(sizeof(char*) * wc);
    
    int i;
    // Copy word tokens into the dynamically allocated strings
    char* token = strtok(phr, " ");
    for (i = 0; i < wc; i++){
        words[i] = (char*)malloc(strlen(phr));
        strncpy(words[i], token, strlen(token));
        token = strtok(NULL, " ");
    }
    return words;
}

/***********************************************************
 searchPhrase()
 
 Searches the file at the path, for each word. If any word is missing, it returns 0.
 ***********************************************************/
int searchPhrase(char** words, int wc, char* path){
    // Search the file at "path" for each word, if a word is not found, return 0
    int i;
    for (i = 0; i < wc; i++)
        if (searchWord(words[i], path) == 0)
            return 0;
    
    return 1;
}

/***********************************************************
 searchWord()
 
 If a word is found at the file at path, then return 1
 ***********************************************************/
int searchWord(char* wrd, char* path){
    
    // Open the directory where all files are stored
    FILE* fp = fopen(path, "r");
    
    // Each line of the file is a string stored in rbuf
    char* rbuf = NULL;
    size_t sb = 0;
    ssize_t nr;
    
    char* token;
    // Read the each line of the file into rbuf
    while(( nr = getline(&rbuf, &sb, fp)) != -1 ){
        token = strtok(rbuf, " ");
        while (token != NULL){      // Check each each word in file for a match
            if (strncmp(token, wrd, strlen(wrd)) == 0){
                return 1;
            }
            token = strtok(NULL, " ");
        }
    }
    return 0;
}




