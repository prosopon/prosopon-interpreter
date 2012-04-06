%{
#include "prosopon.h"

#include "pro_expr.h"
#include "pro_expressions.h"
#include "pro_expr_list.h"

#include <stdio.h>


#define YYDEBUG 1

void yyerror(pro_state_ref, const char *p);

extern int yydebug;

%}

%debug

// Pass a prosopon state to the parser and lexer
%parse-param {pro_state_ref state} 
%lex-param   {pro_state_ref state}


%union
{
    int token;
    char* identifier;
    char* string;
    double number;
    struct pro_expr* expr;
}

%token <identifier> IDENTIFIER
%token <string> STRING_LITERAL
%token <number> NUMBER_LITERAL

%token MESSAGE_START
%token MESSAGE_END
%token CONSTRUCTOR_START
%token CONSTRUCTOR_END
%token ACTOR_START
%token ACTOR_END

%token SEND
%token LET
%token BECOME
%token CASE
%token END_CASE_PATTERN
%token END_CASE


%type <expr> literal program statements statement expression definition term send become message
%type <expr> argument_list argument constructor actor behavior case identifier


%start program

%%

literal
    : STRING_LITERAL
    {
        $$ = pro_string_expr_create($1);
    }
    | NUMBER_LITERAL
    {
        $$ = pro_number_expr_create($1);
    }
    ;
    
identifier
    : IDENTIFIER
    {
        $$ = pro_identifier_expr_create($1);
    }
    ;

/**
 * Program
 */
program
    : statements
    {
        pro_print_expr(state, $1, "\n");
        pro_eval_expr(state, $1);
        pro_run(state);
    }
    ;

statements
    : statements statement
    {
        pro_expr_list* list = pro_expr_list_create($2, 0);
        $$ = pro_list_expr_join($1, pro_list_expr_create(list));
    }
    | statement
    {
        pro_expr_list* list = pro_expr_list_create($1, 0);
        $$ = pro_list_expr_create(list);
    }
    ;
    
statement
    : definition
    | term
    ;


/**
 * Definition
 */ 
definition
    : LET identifier actor
    {
        $$ = pro_let_expr_create($2, $3);
    }
    | LET constructor actor
    {
        $$ = pro_let_expr_create($2, $3);
    }
    | LET identifier literal
    {
        $$ = pro_let_expr_create($2, $3);
    }
    ;
    
/**
 * Expression
 */
expression
    : expression term
    {
        pro_expr_list* list = pro_expr_list_create($2, 0);
        $$ = pro_list_expr_join($1, pro_list_expr_create(list));
    }
    | term
    {
        pro_expr_list* list = pro_expr_list_create($1, 0);
        $$ = pro_list_expr_create(list);
    }
    ;

term
    : send
    | become
    | identifier
    | literal
    | constructor
    | actor
    | message
    ;

/**
 * Send
 */
send
    : SEND identifier message
    {
        $$ = pro_send_expr_create($2, $3);
    }
    | SEND literal message
    {
        $$ = pro_send_expr_create($2, $3);
    }
    | SEND actor message
    {
        $$ = pro_send_expr_create($2, $3);
    }
    | SEND constructor message
    {
        $$ = pro_send_expr_create($2, $3);
    }
    ;

/**
 * Become
 */
become
    : BECOME identifier actor
    {
        $$ = pro_become_expr_create($2, $3);
    }
    | BECOME identifier constructor
    {
        $$ = pro_become_expr_create($2, $3);
    }
    ;

/**
 * Message
 */
message
    : MESSAGE_START MESSAGE_END
    {
        $$ = pro_message_expr_create(0);
    }
    | MESSAGE_START argument_list MESSAGE_END
    {
        $$ = pro_message_expr_create($2->value.list);
    }
    ;

argument_list
    : argument_list argument
    {
        pro_expr_list* list = pro_expr_list_create($2, 0);
        $$ = pro_list_expr_join($1, pro_list_expr_create(list));
    }
    | argument
    {
        pro_expr_list* list = pro_expr_list_create($1, 0);
        $$ = pro_list_expr_create(list);
    }
    ;

argument
    : identifier
    | literal
    | message
    | actor
    | constructor
    ;

/**
 * Constructor
 */
constructor
    : IDENTIFIER CONSTRUCTOR_START argument_list CONSTRUCTOR_END
    {
        $$ = pro_constructor_expr_create($1, $3->value.list);
    }
    | IDENTIFIER CONSTRUCTOR_START CONSTRUCTOR_END
    {
        $$ = pro_constructor_expr_create($1, 0);
    }
    ;
    

/**
 * Actor
 */    
actor
    : ACTOR_START ACTOR_END
    {
        $$ = pro_actor_expr_create(0);
    }
    | ACTOR_START behavior ACTOR_END
    {
        $$ = pro_actor_expr_create($2);
    }
    ;

behavior
    : behavior case
    {
        pro_expr_list* list = pro_expr_list_create($2, 0);
        $$ = pro_list_expr_join($1, pro_list_expr_create(list));
    }
    | case
    {
        pro_expr_list* list = pro_expr_list_create($1, 0);
        $$ = pro_list_expr_create(list);
    }
    ;

case
    : CASE argument_list  END_CASE_PATTERN expression   END_CASE
    {
        $$ = pro_case_expr_create($2, $4);
    }
    | CASE argument_list  END_CASE_PATTERN              END_CASE
    {
        $$ = pro_case_expr_create($2, 0);
    }
    | CASE                END_CASE_PATTERN expression   END_CASE
    {
        $$ = pro_case_expr_create(0, $3);
    }
    | CASE                END_CASE_PATTERN              END_CASE
    {
        $$ = pro_case_expr_create(0, 0);
    }
    ;
    
%%

void yyerror(pro_state_ref s, const char *p)
{
    fprintf(stderr, "Error! %s\n", p);
    fflush(stderr);
}

