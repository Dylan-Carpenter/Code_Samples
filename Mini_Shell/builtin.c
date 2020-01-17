/*  $Id: builtin.c,v 1.46 2017/05/30 19:51:28 carpend3 Exp $  */
/*
* Author: Dylan Carpenter
* Date: April 11th, 2017
* CS352-Spring17
* Modified April 13th, 2017
*
*/
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "proto.h"
#include <pwd.h>
#include <grp.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/types.h>
#include <time.h>

//reference to shiftFactor
int shiftFactor;
//reference to mainargcp
int mainargcp;
//reference to exitVal
int exitVal;

//built in exit
void b_exit(char** argv, int* argcp, int outfd, int errfd){
    //if the user has at least one arg pass the first to exit, if not pass 0
    if(*argcp == 2){
        exit(0);
    }else{
        exit(atoi(argv[1]));
    }
}

//built in aecho
void b_aecho(char** argv, int* argcp, int outfd, int errfd){
    int j = 1;

    //check for special cases
    if((argv[1] == NULL)){
        dprintf(outfd, "\n");
        return;
    }
    if((argv[2] == NULL) && (strcmp(argv[1], "-n") == 0)){
        return;
    }

    //step through the args and echo them
    for(j = 1; j < *argcp - 1; j++){
        if(!strcmp(argv[j], "-n")){
            j++;
        }
        if(!strcmp(argv[1], "-n")){
            if(j == (*argcp - 2)){
                dprintf(outfd, "%s", argv[j]);
            }else{
                dprintf(outfd, "%s ", argv[j]);
            }
        }else{
            if(j == (*argcp - 2)){
                dprintf(outfd, "%s\n", argv[j]);
            }else{
                dprintf(outfd, "%s ", argv[j]);
            }
        }

    }
    exitVal = 0;
    return;
}

//built in envset
void b_envset(char** argv, int* argcp, int outfd, int errfd){

    //check for invalid envset usage
    if((argv[1] == NULL) || (argv[2] == NULL)){
        dprintf(errfd, "%s\n", "usage: envset name value");
        exitVal = 1;
        return;
    }

    //if valid usage use setenv
    setenv(argv[1], argv[2], 1);
    exitVal = 0;
}

//built in envunset
void b_envunset(char** argv, int* argcp, int outfd, int errfd){

    //check for invalid envunset usage
    if(argv[1] == NULL){
        dprintf(errfd, "%s\n", "usage: envunset name");
        exitVal = 1;
    }

    //if valid usage of envunset
    unsetenv(argv[1]);
    exitVal = 0;
}

//built in chdir
void b_chdir(char** argv, int* argcp, int outfd, int errfd){
    //look for cd with no args
    if(argv[1] == NULL){
        //check for the "HOME" environment variable
        if(chdir(getenv("HOME")) != 0){
            dprintf(errfd, "%s\n", strerror(errno));
            exitVal = 1;
            return;
        }
        //check to see if the arg is a valid path
    }else if((chdir(argv[1])) != 0){
        dprintf(errfd, "Error accessing directory: %s\n", strerror(errno));
        exitVal = 1;
        return;
        //throw an error if the user puts more than two args
    }else if(argv[2] != NULL){
        dprintf(errfd, "%s\n", "usage: cd [dir]");
        exitVal = 1;
        return;
    }
    exitVal = 0;
    return;
}

//built in shift[n]
void b_shift(char** argv, int* argcp, int outfd, int errfd){
    //create and set default shift factor
    int shiftNum = 1;
    char shiftNumString[10];
    int flag = 0;
    //check the provided shift factor, if valid set accordingly
    if(argv[1] != NULL){
        char* string = argv[1];
        int i = 0;
        int j = 0;
        flag = 1;
        while(isdigit(string[i])){
            shiftNumString[j] = string[i];
            j++;
            i++;
        }
    }

    //update shift
    if(flag){
        shiftNum = atoi(shiftNumString);
        if((shiftNum >= (mainargcp-1)) || (shiftNum < 0)){
            dprintf(errfd, "%s\n", "Shift index out of bounds");
            exitVal = 1;
            return;
        }
    }


    shiftFactor = shiftFactor + shiftNum;
    exitVal = 0;
    return;
}

//built in unshift[n]
void b_unshift(char** argv, int* argcp, int outfd, int errfd){
    //create and set default shift factor
    int shiftNum = shiftFactor;
    char shiftNumString[10];
    int flag = 0;
    if(argv[1] != NULL){
        char* string = argv[1];
        int i = 0;
        int j = 0;
        flag = 1;
        while(isdigit(string[i])){
            shiftNumString[j] = string[i];
            j++;
            i++;
        }
    }

    if(flag){
        shiftNum = atoi(shiftNumString);
        if((shiftNum > shiftFactor) || (shiftNum < 0)){
            dprintf(errfd, "%s\n", "Shift index out of bounds");
            exitVal = 1;
            return;
        }
    }

    //update shiftFactor
    shiftFactor = shiftFactor - shiftNum;
    exitVal = 0;
    return;
}

//built in sstat
void b_sstat(char **argv, int *argcp, int outfd, int errfd){

    if(argv[1] == NULL){
        dprintf(errfd, "%s\n", "stat: no files to stat");
        exitVal = 1;
        return;
    }

    int fileIndex = 1;
    DIR *dir;

    struct dirent *entry;
    char *fileName;
    struct group *group;
    struct passwd *name;
    struct stat info;
    int fileFoundFlag = 0;

    while(argv[fileIndex] != NULL){
        dir = opendir(".");
        //initialize fileFoundFlag
        if(dir){
            stat(argv[fileIndex], &info);
            if(S_ISREG(info.st_mode)){
                //set fileFoundFlag
                //file name
                //fileName = entry->d_name;
                dprintf(outfd,"%s ", argv[fileIndex]);
                //username and group name

                name = getpwuid(info.st_uid);
                if(name == NULL){
                    dprintf(outfd,"%d ", info.st_uid);
                }else{
                    dprintf(outfd,"%s ", name->pw_name);
                }
                group = getgrgid(info.st_gid);
                if(group == NULL){
                    dprintf(outfd,"%d ", info.st_gid);
                }else{
                    dprintf(outfd,"%s ", group->gr_name);
                }
                //permissions
                dprintf(outfd,(S_ISDIR(info.st_mode)) ? "d" : "-");
                dprintf(outfd,(info.st_mode & S_IRUSR) ? "r" : "-");
                dprintf(outfd,(info.st_mode & S_IWUSR) ? "w" : "-");
                dprintf(outfd,(info.st_mode & S_IXUSR) ? "x" : "-");
                dprintf(outfd,(info.st_mode & S_IRGRP) ? "r" : "-");
                dprintf(outfd,(info.st_mode & S_IWGRP) ? "w" : "-");
                dprintf(outfd,(info.st_mode & S_IXGRP) ? "x" : "-");
                dprintf(outfd,(info.st_mode & S_IROTH) ? "r" : "-");
                dprintf(outfd,(info.st_mode & S_IWOTH) ? "w" : "-");
                dprintf(outfd,(info.st_mode & S_IXOTH) ? "x " : "- ");

                //links
                dprintf(outfd,"%lu ", info.st_nlink);

                //size
                dprintf(outfd,"%lu ", info.st_size);

                //modification time
                dprintf(outfd,"%s", ctime(&info.st_mtime));

            }else{
                fileFoundFlag = 1;
            }
        }
        fileIndex++;
    }
    if(fileFoundFlag){
        exitVal = 1;
    }else{
        exitVal = 0;
    }

    closedir(dir);
}

//built in read function
void b_read(char** argv, int *argcp, int outfd, int errfd){

    char input[1024];
    fgets(input, 1024, stdin);

    int i = 0;
    while(input[i] != '\n'){i++;}
    input[i] = '\0';

    setenv(argv[1], input, 1);
}

//define a function pointer that takes char** and int*
typedef void (*funcptr) (char**, int*, int, int);

//functoin thats checks if builtin.c contains a command the user wants to use
int check_builtin(char** argv, int *argcp, int outfd, int errfd){
    //create an array of the names of the commands in builtin.c
    char* methods[10] = {"exit","aecho","envset","envunset","cd","shift","unshift","sstat","read",NULL};
    //create a function pointer array with space for available commands
    funcptr flist[9];
    //populate flist with internal function names in the same order as methods
    flist[0] = b_exit;
    flist[1] = b_aecho;
    flist[2] = b_envset;
    flist[3] = b_envunset;
    flist[4] = b_chdir;
    flist[5] = b_shift;
    flist[6] = b_unshift;
    flist[7] = b_sstat;
    flist[8] = b_read;
    //create counter
    int i = 0;
    //look through the elements of methods
    while(methods[i] != NULL){
        //if methods contains the command the user wants call it and return 1(true)
        if(!(strcmp(methods[i], argv[0]))){
            flist[i](argv, argcp, outfd, errfd);
            return 1;
        }
        i++;
    }
    //otherwise return 0 (command the user wants is not in builtin.c)
    return 0;
}
