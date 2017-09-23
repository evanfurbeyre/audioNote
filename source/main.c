

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include "json.h"
#include "create.h"
#include "search.h"


int main(int argc, const char * argv[]) {
    srand((unsigned)time(NULL));

    int loop = 1;
    while (loop){                           // MAIN MENU
        printf("-----------------------------------\nAUDIO NOTE:\n");
        printf("    1) Run\n");
        printf("    2) Pages\n");
        printf("    3) Exit\n");
        printf("-----------------------------------\n");
        
        int option = getc(stdin) - 48;      // Get menu selection from user
        getc(stdin);                        // remove extra \n

        switch(option){
            // RUN ALL - convert voice memos to text files
            case 1: {
                
                //MOVE FILES FROM IPHONE VOICE MEMO FOLDER OVER TO all_audio FOLDER
                forkExec("./scripts/phoneGrab.sh");
                
                // CONVERT AUDIO FILES FROM aif/m4a TO flac
                printf("Converting Audio... ");
                forkExec("./scripts/convAudio.sh");
                moveFiles("all_audio", "old/all_audio");
                printf("Done\n");
                
                // RUN SCRIPT TO SEND AUDIO TO API
                printf("Sending/Receiving Audio From API... ");
                fflush(stdout);
                forkExec("./scripts/sendAudio.sh");
                moveFiles("flac_audio", "old/flac_audio");
                printf("Done\n");
                
                // CONVERT THE API RESPONSE FROM JSON TO PLAINTEXT
                printf("Converting Response... ");
                forkExec("./scripts/convJson.sh");
                moveFiles("json_response", "old/json_response");
                printf("Done\n");
                
                // CONFIRM PLAINTEXT
                forkExec("./scripts/confirmText.sh");
                
                // CREATE FILES, BY INTERPRETING THE PLAINTEXT FILES
                printf("Creating Pages... ");
                readParseCreate();
                moveFiles("plaintext", "old/plaintext");
                printf("Done\n");
                break;
            }
            
                
            // FILES - Display/search/edit files
            case 2: {
                int loopDisp = 1;
                while (loopDisp == 1){
                    printf("-----------------------------------\n");
                    printf("View Files:\n");
                    printf("    1) Display All\n");
                    printf("    2) Search By\n");
                    printf("    3) Exit\n");
                    printf("-----------------------------------\n");
                    
                    int optDisp = getc(stdin) - 48;      // Get menu selection from user
                    getc(stdin);                        // remove extra \n
                    
                    switch(optDisp){
                        case 1: {
                            // Print all files, oldest first
                            printAll();
                            break;
                        }
                        case 2: {
                            // Search Files
                            searchBy();
                            break;
                        }
                        case 3: {
                            printf("Exiting...\n");
                            loopDisp = 0;
                            break;
                        }
                        default: {
                            printf("Please enter a valid option\n\n");
                        }
                    }
                }
                break;
            }

            // EXIT
            case 3: {
                printf("Exiting...\n");
                loop = 0;
                break;
            }
            default: {
                printf("Please enter a valid option\n\n");
            }
        }
    }
    
    return 0;
}
