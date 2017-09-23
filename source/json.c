

#include "json.h"

/***********************************************************
 forkExec()
 
 moves all files from source folder to dest folder
 ***********************************************************/
void forkExec(char* src){
    // SIMPLE FORK/EXEC TO CALL convAudio ON EACH FILE
    pid_t sPid = -9;        // Holds result of fork, 0 for child, childPID for parent
    int chExStat = -9;      // Child exit status
    sPid = fork();              // Fork Here!
    switch(sPid){
        case -1: {       // ERROR
            perror("Fork error!\n"); fflush(stdout);
            exit(1); break;
        }
        case 0: {        // CHILD
            execlp(src, src, NULL);
        }
        default:{        // PARENT
            sPid = waitpid(sPid, &chExStat, 0);      // Clean up child process
            if (sPid == -1) {perror("Error in calling script\n"); exit(1);}
        }
    }
}

void moveFiles(char* iFol, char* oFol){
    // SIMPLE FORK/EXEC TO CALL convAudio ON EACH FILE
    pid_t sPid = -9;        // Holds result of fork, 0 for child, childPID for parent
    int chExStat = -9;      // Child exit status
    sPid = fork();              // Fork Here!
    switch(sPid){
        case -1: {       // ERROR
            perror("Fork error!\n"); fflush(stdout);
            exit(1); break;
        }
        case 0: {        // CHILD
            execlp("./scripts/moveFiles.sh", "./scripts/moveFiles.sh", iFol, oFol, NULL);
        }
        default:{        // PARENT
            sPid = waitpid(sPid, &chExStat, 0);      // Clean up child process
            if (sPid == -1) {perror("Error in moving files\n"); exit(1);}
        }
    }
}