#include "pro_interpreter_state.h"

#include <string.h>
#include <stdio.h>
#include <dirent.h>


extern int yydebug;
extern int yy_flex_debug;
extern FILE* yyin;
extern int yyparse(pro_state_ref);


PRO_INTERPRETER pro_interpreter_state* pro_interpreter_state_create(pro_alloc* alloc)
{
    pro_interpreter_state* t = alloc(0, sizeof(*t));
    memset(t, 0, sizeof(*t));
    t->alloc = alloc;
    
    // process command line arguments
    pro_state_ref state;
    if (pro_state_create(alloc, &state) != PRO_OK)
        return 0;
    
    t->state = state;
    
    return t;
}

PRO_INTERPRETER void pro_interpreter_state_release(pro_interpreter_state* t)
{
    pro_alloc* alloc = pro_interpreter_state_get_alloc(t);

    pro_state_release(t->state);
    
    alloc(t, 0);
}

PRO_INTERPRETER pro_alloc* pro_interpreter_state_get_alloc(pro_interpreter_state* t)
{
    pro_alloc* alloc;
    pro_get_alloc(t->state, &alloc);
    return alloc;
}


PRO_INTERPRETER void pro_interpreter_state_set_debug(
    pro_interpreter_state* t, int val)
{
    t->flags.debug = val;
}

PRO_INTERPRETER void pro_interpreter_state_set_yacc_debug(
    pro_interpreter_state* t, int val)
{
    yydebug = val;
    t->flags.yacc_debug = val;
}

PRO_INTERPRETER void pro_interpreter_state_set_flex_debug(
    pro_interpreter_state* t, int val)
{
    yy_flex_debug = val;
    t->flags.flex_debug = val;
}


PRO_INTERPRETER int pro_eval(pro_interpreter_state* istate, const char* file)
{
    if ((yyin = fopen(file, "r")))
    {
        pro_state_ref s = istate->state;
        int status = yyparse(s);
        fclose(yyin);
        return status;
    }
    return -1;
}


PRO_INTERPRETER int (pro_process_library) (pro_interpreter_state* istate, const char* file)
{
    if (pro_library_load(istate->state, file) == PRO_OK)
        return 0;
    return 1;
}


PRO_INTERPRETER int (load_stdlib_library) (pro_interpreter_state* istate, const char* path)
{
    DIR* dir = opendir(path);
    if (!dir)
        return -1;
        
    char buffer[1024];
    
    for (struct dirent* entry; (entry = readdir(dir)); )
    {
        size_t len = strlen(entry->d_name);
        if (len >= 4)
        {
            if (strcmp (".pro", &(entry->d_name[len - 4])) == 0)
            {
                snprintf(buffer, 1024, "%s/%s", path, entry->d_name);
                if (pro_eval(istate, buffer))
                    return -1;
            }
        }
    }

    closedir(dir);
    return 0;
}
