SHELL=/bin/bash

#
#	define version of c compiler, linker, and lex
#
CC = gcc
LINK = gcc
LEX = flex
BISON = bison

#
#	define yacc lex and compiler flags
#
BISON_FLAGS = --debug -d
LEX_FLAGS = -dv -ll
CFLAGS = -g

SRC = src/pro_actor.c src/pro_behavior.c src/pro_env.c src/pro_lookup.c \
    src/pro_object.c src/pro_state.c
    
OBJ = pro_actor.o pro_behavior.o pro_env.o pro_lookup.o pro_object.o pro_state.o

all : $(SRC)
    gcc -c -fPIC $(SRC) -o $(obj)
    gcc -shared -Wl,-soname,libprosopon.so.1 -o libprosopon.so.1.0.1 $(OBJ)

clean :
    rm -f gram.tab.* lex.yy.c *.o prosopon
    rm -rf prosopon.dSYM



