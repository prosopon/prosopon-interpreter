#include "prosopon_interpreter.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern FILE* yyin;
extern int yyparse(pro_state_ref);


PRO_INTERPRETER int pro_eval(pro_state_ref s, const char* file)
{
    if ((yyin = fopen(file, "r")))
    {
        int status = yyparse(s);
        fclose(yyin);
        return status;
    }
    return -1;
}
