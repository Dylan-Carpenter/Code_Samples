#include <stdio.h>

int checkComment(int start, char *line, char c){

  int index = start;

  int quoteFlag = 0;

  while(line[index] != '\0'){
    if(line[index] == '\"'){
      quoteFlag = !quoteFlag;
    }else if((line[index] == c) && (!quoteFlag)){
      return index;
      dprintf(1,"%d\n",index);
    }
    index++;
  }
  return -1;
}
