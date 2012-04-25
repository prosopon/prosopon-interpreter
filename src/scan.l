%{

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "gram.tab.h"


void slComment(void);
void mlComment(void);

void singleQuotedStringLiteral(char delimiter);
void tripleQuotedStringLiteral(char delimiter);

char* identifier(const char* yytext, unsigned long yyleng);

char* string_literal(const char* yytext, unsigned long yyleng, unsigned int clip);
char* single_quoted_string_literal(const char* yytext, unsigned long yyleng);
char* triple_quoted_string_literal(const char* yytext, unsigned long yyleng);

int hex_number(const char* yytext, unsigned long yyleng);
int decimal_number(const char* yytext, unsigned long yyleng);
int float_number(const char* yytext, unsigned long yyleng);

%}
 
digit         [0-9]
neg           \-
letter        [a-zA-Z_$\?\*]
hex_digit     [a-fA-F0-9]
E             [Ee][+-]?{digit}+
whitespace    [ \t\v\n\f]

%%

    /* Multi Line comments */
"\/\*"                      { mlComment(); }

    /* Single Line comments */
"//"                        { slComment(); }

    /* Triple Quoted String */
\'\'\'(\\.|[^\\'])*\'\'\'   { yylval.string = triple_quoted_string_literal(yytext, yyleng); return STRING_LITERAL; }
\"\"\"(\\.|[^\\"])*\"\"\"   { yylval.string = triple_quoted_string_literal(yytext, yyleng); return STRING_LITERAL; }

    /* Single Quoted String */
\'(\\.|[^\\'])*\'           { yylval.string = single_quoted_string_literal(yytext, yyleng); return STRING_LITERAL; }
\"(\\.|[^\\"])*\"           { yylval.string = single_quoted_string_literal(yytext, yyleng); return STRING_LITERAL; }

    /* Special Symbols */

"("                             { return MESSAGE_START; }
")"                             { return MESSAGE_END; }
"{"                             { return CONSTRUCTOR_START; }
"}"                             { return CONSTRUCTOR_END; }
"<"                             { return ACTOR_START; }
">"                             { return ACTOR_END; }
"\\"                             { return CAPTURE; }

"send"                          { return SEND; }
"let"                           { return LET; }
"become"                        { return BECOME; }
"case"                          { return CASE; }
":"                             { return END_CASE_PATTERN; }
";"                             { return END_CASE; }

    /* Number Literals */
0[xX]{hex_digit}+                   { yylval.number = hex_number(yytext, yyleng); return NUMBER_LITERAL; }
{neg}?{digit}+                      { yylval.number = decimal_number(yytext, yyleng); return NUMBER_LITERAL; }
{neg}?{digit}+{E}?                  { yylval.number = float_number(yytext, yyleng); return NUMBER_LITERAL; }
{neg}?{digit}*"."{digit}+({E})?     { yylval.number = float_number(yytext, yyleng); return NUMBER_LITERAL; }
{neg}?{digit}+"."{digit}*({E})?     { yylval.number = float_number(yytext, yyleng); return NUMBER_LITERAL; }

    /* Identifier */
{letter}({letter}|{digit})*     { yylval.identifier = identifier(yytext, yyleng); return IDENTIFIER; }


{whitespace}+                { } /* skip whitespace */
.                           { }

%%

int yywrap(void)
{
    return 1;
}

void slComment(void)
{
    char c;
    while ((c = input()) && c != '\n')
        ;
    return;
}

void mlComment(void)
{
    for (char c = input(); c; c = input())
    {
        if (c == '*')
        {
            char n = input();
            if (n == '/')
                return;
        }
    }
}



char* identifier(const char* yytext, unsigned long yyleng)
{
    if (yyleng > 0)
    {
        char* str = malloc(sizeof(*str) * (yyleng + 1));
        memcpy(str, yytext, yyleng + 1);
        return str;
    }
    else
        return 0;
}



char* string_literal(const char* yytext, unsigned long yyleng, unsigned int clip)
{
    unsigned long new_length = yyleng - clip * 2;
    if (new_length > 0)
    {
        char* str = malloc(sizeof(*str) * (new_length + 1));
        memcpy(str, yytext + clip, new_length);
        str[new_length] = 0;
        return str;
    }
    else
        return 0;
}

char* single_quoted_string_literal(const char* yytext, unsigned long yyleng)
{
    return string_literal(yytext, yyleng, 1);
}

char* triple_quoted_string_literal(const char* yytext, unsigned long yyleng)
{
    return string_literal(yytext, yyleng, 3);
}

int hex_number(const char* yytext, unsigned long yyleng)
{
    int t;
    sscanf(yytext, "%x", &t);
    return t;
}

int decimal_number(const char* yytext, unsigned long yyleng)
{
    int t;
    sscanf(yytext, "%d", &t);
    return t;
}

int float_number(const char* yytext, unsigned long yyleng)
{
    float t;
    sscanf(yytext, "%f", &t);
    return (int)t;
}




