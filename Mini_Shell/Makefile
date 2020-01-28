#		$Id: Makefile,v 1.3 2017/05/17 20:01:48 carpend3 Exp $
#
#Author: Dylan Carpenter
#Date: April 20th, 2017

#Macros for files
SRCS = msh.c arg_parse.c builtin.c expand.c checkComment.c
OBJS = ${SRCS:.c=.o}

#output
msh: $(OBJS)
	gcc -g -o msh -Wall $(OBJS)

#object files
msh.o: msh.c
	gcc -g -c $(SRCS)

#clean
clean:
	rm -r msh $(OBJS)
