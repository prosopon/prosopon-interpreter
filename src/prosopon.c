#include "prosopon.h"

#include <stdio.h>
#include <string.h>


extern FILE* yyin;
extern int yyparse(pro_state*);
extern int yydebug;


static int process_file(pro_state* state, const char* arg)
{
    if ((yyin = fopen(arg, "r")))
    {
        int status = yyparse(state);
        fclose(yyin);
        return status;
    }
    return -1;
}


static int process_flag(pro_state* state, const char* flag)
{
    if (strcmp(flag, "yydebug") == 0)
    {
        yydebug = 1;
        return 0;
    }
    
    return -1;
}


static int process_args(pro_state* state, const char* arg)
{
    size_t len = strlen(arg);
    if (len == 0)
        return 0;
        
    switch (arg[0])
    {
    case '-':   return process_flag(state, arg + 1);
    default:    return process_file(state, arg);
    }
}


int main(int argc, char** argv)
{
    yydebug = 0;

    pro_state* state = pro_state_create();
    for (unsigned int i = 1; i < argc; ++i)
    {
        int status = process_args(state, argv[i]);
        if (status != 0)
            return -1;
    }
    
    return 0;
}
