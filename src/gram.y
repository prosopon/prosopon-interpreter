%{
#include "prosopon.h"

#include "pro_expr.h"
#include "pro_expressions.h"
#include "pro_expr_list.h"

#include <stdio.h>


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
%token CAPTURE

%token SEND
%token LET
%token BECOME
%token CASE
%token END_CASE_PATTERN
%token END_CASE


%type <expr> literal identifier capture_identifier
%type <expr> program statements statement expression definition term
%type <expr> message value_list value
%type <expr> send send_message send_target
%type <expr> become become_target
%type <expr> actor argument_list argument constructor  behavior case  


%start program

%%

literal
    : STRING_LITERAL
    {
        $$ = pro_string_expr_create(state, $1);
    }
    | NUMBER_LITERAL
    {
        $$ = pro_number_expr_create(state, $1);
    }
    ;
    
identifier
    : IDENTIFIER
    {
        $$ = pro_identifier_expr_create(state, $1);
    }
    ;
    
capture_identifier
    : CAPTURE IDENTIFIER
    {
        $$ = pro_capture_identifier_expr_create(state, $2);
    }
    ;

/**
 * Program
 */
program
    : statements
    {
      //  pro_print_expr(state, $1, "\n");
        pro_release(state, pro_eval_expr(state, $1));
        pro_run(state);
        pro_release_expr(state, $1);
    }
    ;

statements
    : statements statement
    {
        pro_alloc* alloc;
        pro_get_alloc(state, &alloc);
        
        pro_expr_list* expr_list = pro_expr_list_create(state, $2, 0);
        pro_expr* list = pro_list_expr_create(state, expr_list);
        $$ = pro_list_expr_join($1, list);
        
        alloc(list, 0);
    }
    | statement
    {
        pro_expr_list* list = pro_expr_list_create(state, $1, 0);
        $$ = pro_list_expr_create(state, list);
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
        $$ = pro_let_expr_create(state, $2, $3);
    }
    | LET identifier constructor
    {
        $$ = pro_let_expr_create(state, $2, $3);
    }
    | LET constructor actor
    {
        $$ = pro_let_expr_create(state, $2, $3);
    }
    | LET identifier literal
    {
        $$ = pro_let_expr_create(state, $2, $3);
    }
    ;
    
/**
 * Expression
 */
expression
    : expression term
    {
        pro_alloc* alloc;
        pro_get_alloc(state, &alloc);
        
        pro_expr_list* expr_list = pro_expr_list_create(state, $2, 0);
        pro_expr* list = pro_list_expr_create(state, expr_list);
        $$ = pro_list_expr_join($1, list);
        
        alloc(list, 0);
    }
    | term
    {
        pro_expr_list* list = pro_expr_list_create(state, $1, 0);
        $$ = pro_list_expr_create(state, list);
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
    : SEND send_target send_message
    {
        $$ = pro_send_expr_create(state, $2, $3);
    }
    ;

send_target 
    : identifier
    | literal
    | actor
    | constructor
    ;

send_message
    : message
    | identifier
    ;

/**
 * Become
 */
become
    : BECOME identifier become_target
    {
        $$ = pro_become_expr_create(state, $2, $3);
    }
    ;

become_target
    : actor
    | constructor
    | identifier
    ;

/**
 * Message
 */
message
    : /* empty message */
    MESSAGE_START MESSAGE_END
    {
        $$ = pro_message_expr_create(state, 0);
    }
    | /* non empty message */
    MESSAGE_START value_list MESSAGE_END
    {
        pro_alloc* alloc;
        pro_get_alloc(state, &alloc);
        
        $$ = pro_message_expr_create(state, $2->value.list);
        
        alloc($2, 0);
    }
    ;
    
value_list
    : value_list value
    {
        pro_alloc* alloc;
        pro_get_alloc(state, &alloc);
        
        pro_expr_list* expr_list = pro_expr_list_create(state, $2, 0);
        pro_expr* list = pro_list_expr_create(state, expr_list);
        $$ = pro_list_expr_join($1, list);
        
        alloc(list, 0);
    }
    | value
    {
        pro_expr_list* list = pro_expr_list_create(state, $1, 0);
        $$ = pro_list_expr_create(state, list);
    }
    ;

value
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
    : /* empty constructor */
    IDENTIFIER CONSTRUCTOR_START CONSTRUCTOR_END
    {
        $$ = pro_constructor_expr_create(state, $1, 0);
    }
    | /* non empty constructor */
    IDENTIFIER CONSTRUCTOR_START value_list CONSTRUCTOR_END
    {
        $$ = pro_constructor_expr_create(state, $1, $3->value.list);
        
        pro_alloc* alloc;
        pro_get_alloc(state, &alloc);
        alloc($3, 0);
    }
    ;


/**
 * Actor
 */    
actor
    : /* empty actor */
    ACTOR_START ACTOR_END
    {
        $$ = pro_actor_expr_create(state, 0);
    }
    | /* actor with behavior */
    ACTOR_START behavior ACTOR_END
    {
        $$ = pro_actor_expr_create(state, $2);
    }
    ;

behavior
    : behavior case
    {
        pro_alloc* alloc;
        pro_get_alloc(state, &alloc);
        
        pro_expr_list* expr_list = pro_expr_list_create(state, $2, 0);
        pro_expr* list = pro_list_expr_create(state, expr_list);
        $$ = pro_list_expr_join($1, list);
        
        alloc(list, 0);
    }
    | case
    {
        pro_expr_list* list = pro_expr_list_create(state, $1, 0);
        $$ = pro_list_expr_create(state, list);
    }
    ;

case
    : CASE argument_list  END_CASE_PATTERN expression   END_CASE
    {
        $$ = pro_case_expr_create(state, $2, $4);
    }
    | CASE argument_list  END_CASE_PATTERN              END_CASE
    {
        $$ = pro_case_expr_create(state, $2, 0);
    }
    | CASE                END_CASE_PATTERN expression   END_CASE
    {
        $$ = pro_case_expr_create(state, 0, $3);
    }
    | CASE                END_CASE_PATTERN              END_CASE
    {
        $$ = pro_case_expr_create(state, 0, 0);
    }
    ;
    
argument_list
    : argument_list argument
    {
        pro_alloc* alloc;
        pro_get_alloc(state, &alloc);
        
        pro_expr_list* expr_list = pro_expr_list_create(state, $2, 0);
        pro_expr* list = pro_list_expr_create(state, expr_list);
        
        $$ = pro_list_expr_join($1, list);
        
        alloc(list, 0);
    }
    | argument
    {
        pro_expr_list* list = pro_expr_list_create(state, $1, 0);
        $$ = pro_list_expr_create(state, list);
    }
    ;

argument
    : identifier
    | capture_identifier
    | literal
    | message
    | actor
    | constructor
    ;

%%

void yyerror(pro_state_ref s, const char *p)
{
    fprintf(stderr, "Error! %s\n", p);
    fflush(stderr);
}

