/*  $Id: expand.c,v 1.50 2017/05/29 23:02:03 carpend3 Exp $  */
/*
* Author: Dylan Carpenter
* Date: April 21st, 2017
* CS352-Spring17
*
*/
#include <stdio.h>
#include <ctype.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "proto.h"

/* Constants */
#define LINELEN 200000
#define DONTWAIT 0

/* references to globals */
int mainargcp;
char **mainargvp;
int shiftFactor;
int exitVal;

//helper function for checking if there is a line length overflow
int checkOverflow(int j, int cerrfd){
    //check if the program has written past the length of the buffer
    if(j > (LINELEN - 1)){
        dprintf(cerrfd, "%s\n", "Line length overflow");
        return 1;//fail
    }else{
        return 0;//success
    }
}


//helper function that adds the characters of a new string @ index of new
int replace(char *string, int index, char *new, int cerrfd){
    int sCount = 0;
    //walk through the passed string and add it into the new line
    while(string[sCount] != '\0'){
        new[index] = string[sCount];
        index++;
        if(checkOverflow(index, cerrfd)){
            return 0;
        }
        sCount++;
    }
    return index;
}


//main expand function called in msh.c
int expand(char *orig, char *new, int newsize, int cinfd, int cerrfd){
    //initialize some variables
    int i = 0;
    int j = 0;
    char *tempPointer;
    char *string;
    char pidString[10];
    char *stringWild = NULL;
    char *stWild;
    char save;

    //walk through the line given by the user
    while(orig[i] != '\0'){
        //if orig[i] isnt a dollar sign, write it to new
        if(orig[i] != '$'){
            if((orig[i] == '\\') && (orig[i+1] == '*')){
                i++;
            }
            //check for wildcard
            if(orig[i] == '*'){
                //simple wildcard
                int astIndex = i;
                int saveStart = i;

                while((orig[astIndex] != ' ') && (orig[astIndex] != '\0')){
                    astIndex++;
                }
                if((orig[astIndex-1] == '*') && ((astIndex -1) != i)){
                    if((orig[i] != ' ') || (orig[i] == '*')){
                        new[j] = orig[i];
                        j++;
                        i++;
                        //check for buffer overflow error
                        if(checkOverflow(j, cerrfd)){
                            return 1;
                        }
                    }

                }else if(((orig[i-1] == ' ') || (orig[i-1] == '\"')) && (orig[i+1] != '*')){
                    i++;
                    int wildIndex = i;
                    int wsIndex = 0;
                    int wildFlag = 0;
                    if((orig[wildIndex] != '\0') && (orig[wildIndex] != ' ')){
                        wildFlag = 1;
                        stringWild = &orig[wildIndex];
                        while(((orig[wildIndex] != '\0') && (orig[wildIndex] != ' ')) && (orig[wildIndex] != '\"')){
                            wsIndex++;
                            wildIndex++;
                            i++;
                        }
                    }

                    if(wildFlag){
                        save = orig[wildIndex];
                        orig[wildIndex] = '\0';
                    }

                    DIR *dir;
                    struct dirent *entry;
                    dir = opendir(".");
                    if(dir){
                        string = NULL;
                        int numWrit = 0;
                        while((entry = readdir(dir)) != NULL){
                            if(entry->d_type == DT_REG){
                                string = entry->d_name;
                                int wildPos = strlen(string) - wsIndex;
                                char *tempString = '\0';
                                if(wildFlag){
                                    tempString = string + wildPos;
                                }
                                if(!wildFlag){
                                    if(!(strcmp(string, "*") == 0)){
                                        j = replace(string, j, new, cerrfd);
                                        new[j] = ' ';
                                        j++;
                                    }
                                }else{
                                    if(strcmp(tempString, stringWild) == 0){
                                        if(!(strcmp(string, "*") == 0)){
                                            numWrit++;
                                            j = replace(string, j, new, cerrfd);
                                            new[j] = ' ';
                                            j++;
                                        }

                                    }
                                }
                            }
                        }
                        if(numWrit == 0){
                            i = saveStart-1;
                            new[j] = orig[i];
                            j++;
                            i++;
                            //check for buffer overflow error
                            if(checkOverflow(j, cerrfd)){
                                return 1;
                            }
                        }
                        string = NULL;
                        j--;
                        closedir(dir);
                        if(wildFlag){
                            orig[wildIndex] = save;
                        }

                    }else{
                        new[j] = orig[i];
                        j++;
                        i++;
                        //check for buffer overflow error
                        if(checkOverflow(j, cerrfd)){
                            return 1;
                        }
                    }
                }else{
                    new[j] = orig[i];
                    j++;
                    i++;
                    //check for buffer overflow error
                    if(checkOverflow(j, cerrfd)){
                        return 1;
                    }
                }
            }
            if(orig[i] == '*'){
                i++;
            }else{
                new[j] = orig[i];
                j++;
                i++;
                //check for buffer overflow error
                if(checkOverflow(j, cerrfd)){
                    return 1;
                }
            }

        }else{
            //if orig[i] is a dollar sign, check for the bracket after it
            if(orig[i+1] == '{'){
                i = i + 2;
                //check for closing bracket right after opening bracket
                if(orig[i] == '}'){
                    i++;
                }else{
                    tempPointer = &orig[i];
                    //walk through the rest of the arg until } is found
                    while((orig[i] != '}') && (orig[i] != '\0')){
                        i++;
                    }
                    //check if there is closing bracket
                    if(orig[i] == '\0'){
                        dprintf(cerrfd, "%s\n", "Missing closing bracket");
                        return 1;//fail
                    }
                    orig[i] = '\0';

                    string = getenv(tempPointer);
                    //copy over the character of the environment variable
                    if(string == NULL){
                        orig[i] = '}';
                        i++;
                    }else{
                        j = replace(string, j, new, cerrfd);
                        if(j == 0){
                            return 1;
                        }
                        orig[i] = '}';
                        i++;
                    }
                }
                //check for $$
            }else if(orig[i+1] == '$'){
                sprintf(pidString, "%d", getpid());
                j = replace(pidString, j, new, cerrfd);
                //check if checkOverflow returned 0
                if(j == 0){
                    return 1;
                }
                i = i + 2;
            }else if(isdigit(orig[i+1]) != 0){
                i++;
                int numIndex = 0;
                int flag = 0;
                if(mainargvp[1] == NULL){
                    flag = 1;
                }
                while(isdigit(orig[i])){
                    pidString[numIndex] = orig[i];
                    i++;
                    numIndex++;
                }
                numIndex = atoi(pidString) + shiftFactor;

                if(!(numIndex > (mainargcp-2))){

                    if(mainargcp > 1){
                        numIndex++;
                    }

                    string = mainargvp[numIndex];

                    j = replace(string, j, new, cerrfd);
                    if(j == 0){
                        return 1;
                    }
                }else if(!(numIndex > (mainargcp -1))){
                    if(!((numIndex == (mainargcp  -1)) && (numIndex != 0))){
                        string = mainargvp[numIndex];
                        j = replace(string, j, new, cerrfd);
                        if(j == 0){
                            return 1;
                        }
                    }else{
                        new[j] = orig[i];
                        i++;
                        j++;
                        //check for buffer overflow error
                        if(checkOverflow(j, cerrfd)){
                            return 1;
                        }
                    }

                }


            }else if(orig[i+1] == '#'){
                int argNum = (mainargcp-1) - shiftFactor;
                if(mainargcp == 1){
                    argNum = 1;
                }
                sprintf(pidString, "%d", argNum);
                j = replace(pidString, j, new, cerrfd);
                if(j == 0){
                    return 1;
                }
                i = i + 2;
            }else if(orig[i+1] == '?'){
                int exitNum = exitVal;
                sprintf(pidString, "%d", exitNum);
                j = replace(pidString, j, new, cerrfd);
                if(j == 0){
                    return 1;
                }
                i = i + 2;
            }else if(orig[i + 1] == '('){
                i = i + 2;
                int saveIndex = i;
                int parenCount = 1;

                while((parenCount != 0) && (orig[i] != '\0')){
                    if(orig[i] == '('){
                        parenCount++;
                        i++;
                    }else if(orig[i] == ')'){
                        parenCount--;
                        i++;
                    }else{
                        i++;
                    }
                }

                if(parenCount != 0){
                    dprintf(cerrfd, "ERROR: No matching ')'\n");
                    return 1;
                }else{
                    i--;
                    orig[i] = '\0';
                }

                int pipefd[2];
                pipe(pipefd);
                int x;
                int status;

                x = processline((orig + saveIndex), cinfd, cerrfd, pipefd[1], DONTWAIT);

                if(x == -1){
                    close(pipefd[0]);
                    close(pipefd[1]);
                    dprintf(cerrfd, "ERROR: Processline did a bad thing\n");
                    return -1;
                }

                close(pipefd[1]);
                char c;
                int overflowFlag = 0;

                while(read(pipefd[0], &c, 1) > 0){
                    if(!(j == LINELEN)){
                        if(c == '\n'){
                            new[j] = ' ';
                            j++;
                        }else{
                            new[j] = c;
                            j++;
                        }

                    }else if(!overflowFlag){
                        overflowFlag = 1;
                    }
                }

                if(new[j-1] == ' '){
                    j--;
                }

                if(overflowFlag){
                    dprintf(cerrfd, "ERROR: buffer overflow\n");
                    close(pipefd[0]);
                    return 1;
                }

                close(pipefd[0]);
                orig[i] = ')';
                i++;

                //how to tell a child exists
                if(x > 0){
                    if (wait (&status) < 0)
                    dprintf(cerrfd, "wait");

                    if(WIFEXITED(status)){
                        exitVal = WEXITSTATUS(status);
                    }else if(WIFSIGNALED(status)){
                        exitVal = 127;
                    }
                }

            }else{
                new[j] = orig[i];
                i++;
                j++;
                //check for buffer overflow error
                if(checkOverflow(j, cerrfd)){
                    return 1;
                }
            }
        }
    }
    new[j] = '\0';
    return 0;//success
}
