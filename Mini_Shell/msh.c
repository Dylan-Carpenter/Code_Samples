
/*  $Id: msh.c,v 1.38 2017/06/02 01:07:41 carpend3 Exp $  */

/*  $Id: msh.c,v 1.38 2017/06/02 01:07:41 carpend3 Exp $  */

/* CS 352 -- Mini Shell!
*
*   Sept 21, 2000,  Phil Nelson
*   Modified April 8, 2001
*   Modified January 6, 2003
*   Modified April 4, 2017 - Dylan Carpenter
*
*/


#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "proto.h"


/* Constants */

#define LINELEN 200000
#define WAIT 1
#define DONTWAIT 0
#define NOEXPAND 0
#define EXPAND 2

/* Prototypes */

//int processline (char *line, int outfd, int waitFlag);

/* Shell main */

int mainargcp;
char **mainargvp;
int shiftFactor = 0;
int sigintR = 0;
int exitVal;
FILE *file;


int
main (int mainargc, char **mainargv)
{
    char   buffer [LINELEN];
    int    len;
    file = stdin;
    int fileFlag = 0;
    mainargcp = mainargc;
    mainargvp = mainargv;
    int run = 1;

    void sigIntHandler(int signal){};

    if(mainargc != 1){
        file = fopen(mainargv[1], "r");
        fileFlag = 1;
        if(file == NULL){
            fprintf(stderr, "%s\n", strerror(errno));
            exit(127);
        }
    }

    signal(SIGINT, sigIntHandler);

    while (run) {

        /* prompt */
        if(mainargc == 1){
            fprintf (stderr, "%% ");
        }
        /* and get line */
        if (fgets (buffer, LINELEN, file) != buffer)
        break;

        /* Get rid of \n at end of buffer. */
        len = strlen(buffer);
        if (buffer[len-1] == '\n')
        buffer[len-1] = 0;

        /* Run it ... */
        processline (buffer, 0, 1, 2, WAIT | EXPAND);

    }

    if ((!feof(file)) && (!fileFlag))
    perror ("read");

    return exitVal;
}

/*processline function*/

int processline (char *line, int infd, int outfd, int errfd, int Flags)
{

    int nfd[2]; pipe(nfd); close(nfd[0]); close(nfd[1]);
    pid_t  cpid;
    int    status;
    int argcp;
    char new [LINELEN];
    int newsize = LINELEN;
    int index;

    int cinfd = infd;
    int coutfd = outfd;
    int cerrfd = errfd;

    if((index = checkComment(0,line,'#')) != -1){
        line[index] = '\0';
    }

    if(Flags & EXPAND){
        if(expand(line, new, newsize, cinfd, cerrfd)){
            return -1;
        }
    }else{
        strcpy(new,line);
    }

    //**************************PIPELINES**********************************//
    int start = 0;
    int pipefd[2];
    int readEnd = cinfd;
    if((index = checkComment(start,new,'|')) != -1){
        do{
            new[index] = '\0';
            char* command = &new[start];
            //dprintf(1,"%s\n",command);
            start = index + 1;
            pipe(pipefd);
            processline(command, readEnd, pipefd[1], cerrfd, DONTWAIT | NOEXPAND);
            close(pipefd[1]);
            if(readEnd != infd){
                close(readEnd);
            }
            readEnd = pipefd[0];
            //new[index] = '|';
        }while((index = checkComment(start,new,'|')) != -1);

        char* command = &new[start];
        //dprintf(1,"%s\n",command);
        processline(command, readEnd, outfd, cerrfd, WAIT & Flags);
        close(readEnd);
        while(waitpid(-1, &status, WNOHANG) > 0);

        return 1;
    }

    /*
    int i = 0;
    int pipeLineHappened = 0;
    int pipefd[2];
    int readEnd;
    while(new[i] != '\0'){
        dprintf(1,"%c\n",new[i]);
        if(new[i] == '|'){
            pipeLineHappened = 1;
            int j = i-1;
            while((new[j] != '|') && (j != 0)){
                j--;
            }
            while(new[j] == '|'){
                j++;
            }
            char* command = &new[j];
            readEnd = pipefd[0];

            pipe(pipefd);

            new[i] = '\0';
            dprintf(1,"%s%s\n","command: ",command);
            if(cinfd == 0){
                processline(command, cinfd, pipefd[1], cerrfd, 2);
                dprintf(1,"%s\n","in if");
                dprintf(1,"%s%d\n", "pipefd[0]: ",readEnd);
                dprintf(1,"%s%d\n", "pipefd[1]: ",pipefd[1]);
                close(pipefd[1]);
            }else{
                processline(command, readEnd, pipefd[1], cerrfd, 2);
                dprintf(1,"%s\n","in else");
                dprintf(1,"%s%d\n", "pipefd[0]: ",readEnd);
                dprintf(1,"%s%d\n", "pipefd[1]: ",pipefd[1]);
                close(readEnd);
                close(pipefd[1]);
            }
            new[i] == '|';
        }
        i++;
    }

    //dprintf(1,"%d\n",pipeLineHappened);
    /*
    if(pipeLineHappened){
        pipeLineHappened = 0;
        int l = 0;
        while(new[l] != '|'){
            l--;
        }
        l++;
        char* command = &new[l];
        dprintf(1,"%s%d\n", "pipefd[0]: ",readEnd);
        dprintf(1,"%s%d\n", "pipefd[1]: ",pipefd[1]);
        processline(command, readEnd, coutfd, cerrfd, NOEXPAND | (Flags & 1));
        close(readEnd);
    }
    */

    //**************************REDIRECTIONS*******************************//

    int i = 0;
    int inQuotes = 0;
    //dprintf(1, "{%s}\n", new);
    while(new[i] != '\0'){

        if((new[i] == '\"') && (!inQuotes)){
            inQuotes = 1;
        }else if((new[i] == '\"') && (inQuotes)){
            inQuotes = 0;
        }

        if((new[i] == '>') && (!inQuotes)){
            char* fileName;
            int saveBegin = i;
            int appendFlag = 0;
            int errFlag = 0;

            if((new[i-1] == '2') && (new[i-2] == ' ')){
                errFlag = 1;
                saveBegin = saveBegin - 1;
            }

            if(new[i+1] == '>'){
                appendFlag = 1;
                i++;
            }

            i++;

            while((new[i] == ' ') || (new[i] == '\"')){

                if(new[i] == '\"'){
                    if(inQuotes){
                        inQuotes = 0;
                    }else{
                        inQuotes = 1;
                    }
                    int j = i;
                    while(new[j+1] != '\0'){
                        new[j] = new[j+1];
                        j++;
                    }
                    new[j] = new[j+1];
                }else{
                    i++;
                }

            }


            fileName = &new[i];
            int saveIndex = i;

            //*************SUPER NOT SURE ABOUT THIS CODE****************//

            char saveC;

            while((new[i] != '\0') && (new[i] != '>') && (new[i] != '<')){

                if((new[i] == '\"') && (inQuotes)){
                    inQuotes = 0;
                    int j = i;
                    while(new[j+1] != '\0'){
                        new[j] = new[j+1];
                        j++;
                    }
                    new[j] = new[j+1];
                }else if((new[i] == '\"') && (!inQuotes)){
                    inQuotes = 1;
                    int j = i;
                    while(new[j+1] != '\0'){
                        new[j] = new[j+1];
                        j++;
                    }
                    new[j] = new[j+1];
                }else if((inQuotes) && (new[i] == ' ')){
                    while(new[i] == ' '){
                        i++;
                    }
                }else if((!inQuotes) && (new[i] == ' ')){
                    saveC = new[i];
                    new[i] = '\0';
                }else{
                    i++;
                }
            }

            //dprintf(1, "{%s}\n", new);

            //*************SUPER NOT SURE ABOUT THIS CODE****************//

            /*this code works

            if(!inQuotes){
                while((new[i] != ' ') && (new[i] != '\0') && (new[i] != '>') && (new[i] != '<')){
                    i++;
                }

            }else{
                while((new[i] != '\"') && (new[i] != '\0')){
                    i++;
                }
                if(new[i] == '\"'){
                    inQuotes = 0;
                }
            }

            */


            //char saveC = new[i];
            //new[i] = '\0';

            //handles '>'
            if((!appendFlag) && (!errFlag)){
                int newOutfd = 1;
                if(coutfd != 1){
                    close(coutfd);
                }
                newOutfd = open(fileName, O_CREAT | O_WRONLY | O_TRUNC, 00700);


                if(newOutfd == -1){
                    dprintf(cerrfd, "%s: No such file or directory\n", fileName);
                    return -1;
                }else{

                    coutfd = newOutfd;

                    new[i] = saveC;
                    int j = saveBegin;

                    while(new[i] != '\0'){
                        new[j] = new[i];
                        j++;
                        i++;
                    }

                    new[j] = '\0';
                    i = saveBegin;

                }

            //handles '>>'
            }else if((appendFlag) && (!errFlag)){
                int newOutfd = 1;
                if(coutfd != 1){
                    close(coutfd);
                }
                newOutfd = open(fileName, O_CREAT | O_APPEND | O_WRONLY, 00700);

                if(newOutfd == -1){
                    dprintf(cerrfd, "%s: No such file or directory\n", fileName);
                    return -1;
                }else{
                    coutfd = newOutfd;
                    new[i] = saveC;
                    int j = saveBegin;

                    while(new[i] != '\0'){
                        new[j] = new[i];
                        j++;
                        i++;
                    }

                    new[j] = '\0';
                    i = saveBegin;
                }

                //handles '2>'
            }else if((!appendFlag) && (errFlag)){
                int newErrfd = 2;
                if(cerrfd != 2){
                    close(cerrfd);
                }
                newErrfd = open(fileName, O_CREAT | O_WRONLY | O_TRUNC, 00700);

                if(newErrfd == -1){
                    dprintf(cerrfd, "%s: No such file or directory\n", fileName);
                    return -1;
                }else{
                    cerrfd = newErrfd;
                    new[i] = saveC;
                    int j = saveBegin;

                    while(new[i] != '\0'){
                        new[j] = new[i];
                        j++;
                        i++;
                    }

                    new[j] = '\0';
                    i = saveBegin;
                }

                //handles '2>>'
            }else if((appendFlag) && (errFlag)){

                int newErrfd = 2;
                if(cerrfd != 2){
                    close(cerrfd);
                }
                newErrfd = open(fileName, O_CREAT | O_APPEND | O_WRONLY, 00700);

                if(newErrfd == -1){
                    dprintf(cerrfd, "%s: No such file or directory\n", fileName);
                    return -1;
                }else{
                    cerrfd = newErrfd;
                    new[i] = saveC;
                    int j = saveBegin;

                    while(new[i] != '\0'){
                        new[j] = new[i];
                        j++;
                        i++;
                    }

                    new[j] = '\0';
                    i = saveBegin;
                }

            }

        }else if((new[i] == '<') && (!inQuotes)){
            char* fileName;
            int saveBegin = i;
            i++;

            while(new[i] == ' '){
                i++;
            }

            fileName = &new[i];
            int saveIndex = i;

            while((new[i] != ' ') && (new[i] != '\0')){
                i++;
            }

            char saveC = new[i];
            new[i] = '\0';

            int newInfd = 0;
            if(cinfd != 0){
                close(cinfd);
            }
            newInfd = open(fileName, O_RDONLY, 00700);

            if(newInfd == -1){
                dprintf(cerrfd, "%s: No such file or directory\n", fileName);
                return -1;
            }else{
                cinfd = newInfd;
                new[i] = saveC;
                int j = saveBegin;

                while(new[i] != '\0'){
                    new[j] = new[i];
                    j++;
                    i++;
                }

                new[j] = '\0';
                i = saveBegin;
            }

        }else{
            i++;
        }
    }

    char **argv = arg_parse(new, &argcp);

    //if argv is NULL return and don't fork
    if(argv[0] == NULL){
        return -1;
    }

    //check to see if the command provided by the user in line is built in
    if(check_builtin(argv, &argcp, coutfd, cerrfd)){
        return 0;
    }

    /* Start a new process to do the job. */
    cpid = fork();
    if (cpid < 0) {
        perror ("fork");
        return -1;
    }

    /* Check for who we are! */
    if (cpid == 0) {
        /* We are the child! */
        /* execlp (line, line, (char *)0);*/

        if(coutfd != 1){
            dup2(coutfd, 1);
        }

        if(cinfd != 0){
            dup2(cinfd, 0);
        }

        if(cerrfd != 2){
            dup2(cerrfd, 2);
        }

        execvp(argv[0], argv);
        perror ("exec");
        fclose(file);
        exit (127);
    }

    /* Have the parent wait for child to complete */
    if(Flags & WAIT){
        if (waitpid (cpid, &status, 0) < 0)
        perror ("wait");

        if(WIFEXITED(status)){
            exitVal = WEXITSTATUS(status);
        }else if(WIFSIGNALED(status)){
            exitVal = 127;
        }
    }
    free(argv);
    return cpid;
}
