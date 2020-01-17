/*  $Id: arg_parse.c,v 1.4 2017/04/24 04:45:16 carpend3 Exp $ */
/* CS 352 -- arg_parse!
 *
 *   April 7, 2017 - Dylan Carpenter
 *   Modified April 12th, 2017
 *
 */

 #include <stdio.h>
 #include <string.h>
 #include <unistd.h>
 #include <errno.h>
 #include <stdlib.h>
 #include <sys/types.h>
 #include <sys/wait.h>
 #include "proto.h"


/*arg_parse function*/

char** arg_parse(char* line, int *argcp)
{
    //The following section counts the number of args given by the user in line

    //make initial values and states
    int i = 0;
    int argc = 1;
    int inQuotes = 0;
    int inQArg = 0;
    int quoteCount = 0;

    //while not at the end of line
    while(line[i] != '\0'){
      //skip spaces outside quote
      if((line[i] == ' ') && !(inQuotes)){
        i++;
      //count arg not in quotes
      }else if((line[i] != ' ') && (line[i] != '\"')){
        argc++;
        i++;
        inQArg = 1;
        //skips through rest of the arg
        while((line[i] != ' ') && (line[i] != '\0')){
          if(line[i] == '\"'){//checks for a quote
            inQArg = 0;
            break;//hops out of while if theres a quote
          }else{
            i++;
          }
        }
        inQArg = !(inQArg);
      //finish arg w/ quotes
      }else if(((line[i] == '\"') && (inQuotes)) &&
      ((line[i++] == ' ') || (line[i++] == '\0'))){
        inQuotes = 0;
        i++;
      //if you hit a quote and inQuotes is false
      }else if((line[i] == '\"') && !(inQuotes)){
        inQuotes = 1;
        //if inQArg isnt already true it is set to true
        if(!(inQArg)){
          inQArg = 1;
          argc++;
        }
        i++;
        while(inQArg){
          //check if at end of quote block
          if((inQuotes) && (line[i] == '\"')){
            inQuotes = 0;
            i++;
          //check for space
          }else if(!(inQuotes) && ((line[i] == ' ') || (line[i] == '\0'))){
            inQArg = 0;
            //if you hit a quote and inQuotes is not true
          }else if(!(inQuotes) && (line[i] == '\"')){
            inQuotes = 1;
            i++;
          //skip through rest of arg if not special cases
          }else{
            i++;
          }
        }
      }
    }

    //end of argument counting

    //zero out i
    i = 0;

    //count the number of quotes in line
    while(line[i] != '\0'){
      if(line[i] == '\"'){
        quoteCount++;
        i++;
      }else{
        i++;
      }
    }

    //if there is not an even number of quotes print error and do nothing
    if((quoteCount % 2) != 0){
      printf("%s\n", "ERROR: Invalid number of quotes");
      char** argv = malloc(0);
      return argv;
    }

    //pass argc as argcp by reference
    *argcp = argc;

    //malloc appropriate space for argv
    char** argv = malloc((argc) * sizeof(char*));

    //throw an error if the memory for argv was not properly malloced
    if(argv == NULL){
      perror("cannot read");
    }

    //The following section of code assigns pointers to the elements of argv

    //zero out and set variables for walking through data structures
    i = 0;
    int j = 0;
    int k = 0;

    //while not at the end of the line
    while(line[i] != '\0'){
      //skip any spaces that are outside of an argument
      while(line[i] == ' '){
        i++;
      }
      //if line[i] is inside of an argument
      if((line[i] != ' ') && (line[i] != '\0')){
        argv[k] = &line[j];//point to the first char of the arg in argv
        k++;
        //while line[i] is inside of an argument
        while((line[i] != ' ') && (line[i] != '\0')){
          //if current char is not a quote
          if(line[i] != '\"'){
            line[j] = line[i];//replace line[j] with line[i]
            //increment both of the counters
            j++;
            i++;
            //while current char is still inside an argument and is not a quote
            while(((line[i] != '\"') && (line[i] != ' ')) && (line[i] != '\0')){
              line[j] = line[i];//replace line[j] with line[i]
              //increment both counters
              j++;
              i++;
            }
          }
          //if line[i] is a quote skip over it with i
          if(line[i] == '\"'){
            i++;
            //while the other chars in the arg are not quotes
            while((line[i] != '\"') && (line[i] != '\0')){
              line[j] = line[i];//replace line[j] with line[i]
              //increment both counters
              j++;
              i++;
            }
            i++;
          }
        }
        //if j equals i...
        if((j == i) && (line[j] != '\0')){
          i++;//increment i before j marks the EOS
        }
        //set line[j] as the EOS and increment j
        line[j] = '\0';
        j++;

      }
    }
    //assign the last pointer in argv to NULL
    argv[k] = NULL;

    return argv;
}
