

#include "create.h"

// GLOBAL KEYWORDS
// Causes the following word after "command" to be interpreted
const char* cmd = "control";

// Denotes where a page will begin and end
const char* pge = "page";
const char* end = "done";

// First Order Keywords - these words are simply swapped for their punctuation
const char* prd = "period";
const char* cma = "comma";
const char* etr = "enter";      // Default is double-newline
const char* tab = "tab";

// Second Order Keywords - these effect multiple words, and require a stop keyword
const char* tag = "tag";

// Closes Second Order Keywords
const char* stp = "stop";

/***********************************************************
 readParseCreate()
 
 This function is the main function to handle the creation of pages from plaintext. It reads through the plaintext folder, and for each plaintext file, calls parse
 ***********************************************************/
void readParseCreate(){
    DIR* d = opendir("./plaintext");     // Open the plaintext directory
    struct dirent* dir;
    char path[64];
    if (d){
        while ((dir = readdir(d)) != NULL ){            // Read through all the files in d
            if (strncmp(dir->d_name, ".", 1) != 0){     // Skip . files
                memset(path, '\0', sizeof(path));
                snprintf(path, sizeof(path)-1, "plaintext/%s", dir->d_name);
                parse(path);                         // Parse each file
            }
        }
        closedir(d);
    }else{
        printf("Error opening dir\n");
    }
}

/***********************************************************
 keywordCheck()
 
 Makes sure there are no fatal errors in keyword combinations, 1 if error, 0 if pass
 ***********************************************************/
int keywordErrors(char* txt){
    assert(txt != NULL);
    
    // Check for new page delineator pairs
    char opnpage[32];
    char clopage[32];
    memset(opnpage, '\0', sizeof(opnpage));
    memset(clopage, '\0', sizeof(clopage));
    assert(strlen(cmd) + strlen(pge) + 1 < sizeof(opnpage)-1);  // prevent array overflow
    assert(strlen(cmd) + strlen(end) + 1 < sizeof(clopage)-1);
    sprintf(opnpage, "%s %s", cmd, pge);
    sprintf(clopage, "%s %s", cmd, end);
    if (strstr(txt, opnpage) != NULL){     // If input has "control tag" but no "stop", error = true
        if (strstr(txt, clopage) != NULL){
            return 0;
        } else {
            printf("Missing \"done\" keyword in text\n");
            return 1;
        }
    }
    
    // Check for tagging delineator pairs
    char opntag[32];
    memset(opntag, '\0', sizeof(opntag));
    assert(strlen(cmd) + strlen(tag) + 1 < sizeof(opntag)-1);
    sprintf(opntag, "%s %s", cmd, tag);
    if (strstr(txt, opntag) != NULL){     // If input has "control tag" but no "stop", error = true
        if (strstr(txt, stp) != NULL){
            return 0;
        } else {
            printf("Missing \"stop\" keyword in text\n");
            return 1;
        }
    }
    return 0;
}


/***********************************************************
 parse()
 
 Reads from the file at path, and prints to the write file, one word at a time. If the word "command" is encountered, the next word will be interpreted in the command() function to do things.
 ***********************************************************/
void parse(char* path){
    // Read the input file into buf
    FILE* fpr = fopen(path, "r");
    
    // Get the Date from the path of file
    char* dateId = strstr(path,"2017");
    
    char* buf = NULL;
    size_t sb;
    getline(&buf, &sb, fpr);
    
    if (keywordErrors(buf) == 0){
        // The file to write to. Sends text to dev/null by default
        FILE* fpw = fopen("/dev/null", "w");
        
        char* token = strtok(buf, " ");     // Start the tokenizer at the first word in buf
        while (token != NULL){
            if (strncmp(token, cmd, strlen(cmd)) == 0){     // If keyword cmd, begin interpretation
                fpw = command(token, fpw, dateId);  // Updates the write file pointer, depending on keywords
            }else{
                fprintf(fpw,"%s ", token);  // If regular word, just print it to file
            }
            token = strtok(NULL, " ");  // Next word
        }
        fclose(fpr);    // Close the read file
    }
}

/***********************************************************
 command()
 
 Calls all the functions that handle keywords that follow "command"
 ***********************************************************/
FILE* command(char* tkn, FILE* fp, char* dateId){
    tkn = strtok(NULL, " ");        // Move the tkn one word
    fp = page(tkn, fp, dateId);     // If the word is "page", get a new write file pointer
    punct(tkn, fp);                 // Interpret and insert punctuation/formatting keywords
    tagify(tkn, fp);                // Adds # to words between tag and stop
    endpage(tkn, fp, dateId);               // Closes the current page file
    return fp;
}

/***********************************************************
 page()
 
 Creates a new file with the pid and a random 4 digit number, and returns the updated fp (if tkn == "page")
 ***********************************************************/
FILE* page(char* tkn, FILE* fp, char* dateId){
    if (strncmp(tkn, pge, strlen(pge)) == 0){
        char filename[64];
        memset(filename, '\0', sizeof(filename));
        assert(strlen("page/page") + strlen(dateId) + 1 < sizeof(filename)-1);
        sprintf(filename, "pages/page%s", dateId);
        fp = fopen(filename, "w");
        fprintf(fp, "-----------------------------------\n");
    }
    return fp;
}

/***********************************************************
 punct()
 
 Replaces the punctuation/formatting keyword with equivalent character
 ***********************************************************/
void punct(char* tkn, FILE* fp){
    if (strncmp(tkn, etr, strlen(etr)) == 0){
        fprintf(fp, "\n\n");
    }
    if (strncmp(tkn, prd, strlen(prd)) == 0){
        fprintf(fp, ".");
    }
    if (strncmp(tkn, tab, strlen(tab)) == 0){
        fprintf(fp, "\t");
    }
    if (strncmp(tkn, cma, strlen(cma)) == 0){
        fprintf(fp, ",");
    }
}

/***********************************************************
 tagify()
 
 Add a # to the beginning of all words between keywords tag and stp
 ***********************************************************/
void tagify(char* tkn, FILE* fp){
    if (strncmp(tkn, tag, strlen(tag)) == 0){
        tkn = strtok(NULL, " ");
        while ( tkn != NULL && strncmp(tkn, stp, strlen(stp)) != 0 ){
            fprintf(fp, "#%s ", tkn);
            tkn = strtok(NULL, " ");
        }
        fprintf(fp, "\n");
    }
}


/***********************************************************
 datePrint()
 
 Converts the Apple voice memo date style "20170825_120000" to "12:00 on 08/25/2017"
 ***********************************************************/
void datePrint(FILE* fp, char* dateId){
    fprintf(fp, "\n%.2s:%.2s on ", strstr(dateId, "_")+1, strstr(dateId, "_")+3);
    fprintf(fp, "%.2s/%.2s/%.4s\n", dateId+4, dateId+6, dateId);
}


/***********************************************************
 endpage()
 
 Add a newline to the file, the close up
 ***********************************************************/
void endpage(char* tkn, FILE* fp, char* dateId){
    if (strncmp(tkn, end, strlen(end)) == 0){
        datePrint(fp, dateId);
        fprintf(fp, "-----------------------------------\n\n");
        fclose(fp);
    }
}

