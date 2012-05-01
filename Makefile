SHELL=/bin/bash

UNAME := $(shell uname)

prefix = /usr/local
exec_prefix = $(prefix)
libdir = $(exec_prefix)/lib
includedir = $(prefix)/include


ifeq ($(UNAME), Darwin)
    LIBTOOL = glibtool --tag="junk"
else
    LIBTOOL = libtool
endif
CC = gcc
LINK = gcc
LEX = flex
BISON = bison
DOC = doxygen

BISON_FLAGS = --debug -d
LEX_FLAGS = -dv -ll
CFLAGS = -std=c99 -g -I./include -I./build -I./src
LFLAGS = -lprosopon -lprosopon-stdlib

SRC_DIR = src
SRC_INCLUDE_DIR = include/prosopon

LIBPROSOPON_INTERPRETER = libprosopon-interpreter.la
PROSOPON_HEADER_DIR = $(includedir)/prosopon


OBJS = pro_actor_expr pro_become_expr pro_case_expr pro_constructor_expr pro_expr \
    pro_expr_list pro_expr_type pro_identifier_expr pro_let_expr pro_list_expr \
    pro_message_expr pro_number_expr pro_send_expr pro_string_expr  \
    pro_capture_identifier_expr pro_interpreter_state pro_expressions lex.yy gram.tab

OUT_DIR = build
OUT_OBJS = $(addprefix $(OUT_DIR)/,$(OBJS))


HEADERS = interpreter.h interpreter_config.h
OUT_HEADERS = $(addprefix $(SRC_INCLUDE_DIR)/,$(HEADERS))


all : $(LIBPROSOPON_INTERPRETER)

$(LIBPROSOPON_INTERPRETER) : $(addsuffix .lo,$(OUT_OBJS))
	$(LIBTOOL) --mode=link gcc $(CFLAGS) -O -o $@  $^ -rpath $(libdir) $(LFLAGS)

$(OUT_DIR)/%.lo : $(SRC_DIR)/%.c
	$(LIBTOOL) --mode=compile gcc $(CFLAGS) -fPIC -c $^ -o $@

$(OUT_DIR)/lex.yy.lo : $(OUT_DIR)/lex.yy.c $(OUT_DIR)/gram.tab.lo
	$(LIBTOOL) --mode=compile gcc $(CFLAGS) -fPIC -c $+ -o $@

$(OUT_DIR)/gram.tab.lo : $(OUT_DIR)/gram.tab.c
	$(LIBTOOL) --mode=compile gcc $(CFLAGS) -fPIC -c $^ -o $@

$(OUT_DIR)/lex.yy.c : $(SRC_DIR)/scan.l
	$(LEX) $(LEX_FLAGS) -o $@ $<

$(OUT_DIR)/gram.tab.c : $(SRC_DIR)/gram.y
	$(BISON) $(BISON_FLAGS) -o $@ $<


install: $(LIBPROSOPON_INTERPRETER) copy_headers	
	$(LIBTOOL) --mode=install cp $< $(libdir)/$<

copy_headers: 
	if [ ! -d $(PROSOPON_HEADER_DIR) ]; then mkdir $(PROSOPON_HEADER_DIR); fi
	cp $(OUT_HEADERS) $(PROSOPON_HEADER_DIR)
	

.PHONY : doc
doc :
	doxygen Doxyfile


.PHONY : clean
clean :
	rm -f $(OUT_DIR)/*
	if [ -f libprosopon-interpreter.so.1 ]; then rm libprosopon-interpreter*; fi

