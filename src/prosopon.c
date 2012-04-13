#include "prosopon.h"

#include "prosopon_interpreter.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


extern FILE* yyin;
extern int yyparse(pro_state_ref);
extern int yydebug;
extern int yy_flex_debug;

typedef enum
{
    CL_STATE_FLAG_NONE = 0,
    LOAD_LIBRARY_CL_STATE_FLAG
} cl_state_flag;


typedef struct file_list file_list;
struct file_list
{
    file_list* next;
    const char* file;
};

static file_list* file_list_new(const char* file, file_list* next)
{
    file_list* l = malloc(sizeof(*l));
    l->file = file;
    l->next = next;
    return l;
}


typedef struct cl_state cl_state;
struct cl_state
{
    int load_standard_library;
    file_list* libraries;
    
    file_list* files;

    cl_state_flag flag;
};


/**
 *
 */
static int process_library(pro_state_ref state, const char* file)
{
    if (pro_library_load(state, file) == PRO_OK)
        return 0;
    return 1;
}


/**
 *
 */
static int process_file(pro_state_ref state, const char* arg)
{
    return pro_eval(state, arg);
}


/**
 *
 */
static int process_flag(cl_state* cl, pro_state_ref state, const char* flag, size_t len)
{
    if (len == 0)
        return -1;
    else if (len == 1)
    {
        switch (flag[0])
        {
        case 'l':
            cl->flag = LOAD_LIBRARY_CL_STATE_FLAG;
            return 0;
        default: return -1;
        }
    }
    else
    {
        if (strcmp(flag, "yydebug") == 0)
        {
            yydebug = 1;
            return 0;
        }
        else if (strcmp(flag, "yy_flex_debug") == 0)
        {
            yy_flex_debug = 1;
            return 0;
        }
        else if (strcmp(flag, "no_std") == 0)
        {
            cl->load_standard_library = 0;
            return 0;
        }
        return -1;
    }
    
    return 0;
}

/**
 * Processes a single command line argument.
 * 
 * Arguments beginning with '-' are processed as flags.
 * Everything else is processed depending on the current state. 
 */
static int process_args(cl_state* cl, pro_state_ref state, const char* arg)
{
    size_t len = strlen(arg);
    if (len == 0)
        return 0;
        
    switch (arg[0])
    {
    case '-':
        return process_flag(cl, state, arg + 1, len - 1);
    default:
        switch (cl->flag)
        {
        case CL_STATE_FLAG_NONE:
        {
            file_list* t = file_list_new(arg, 0);
            if (!cl->files)
                cl->files = t;
            else
            {
                file_list* last = cl->files;
                while (last->next)
                    last = last->next;
                last->next = t;
            }
            return 0;
        }   break;
        case LOAD_LIBRARY_CL_STATE_FLAG:
            cl->flag = CL_STATE_FLAG_NONE;
            return process_library(state, arg);
        default: return -1;
        }
    }
}



/**
 * Main function to invoke the interpreter. The command takes the form
 * 
 * prosopon [-yydebug] [-yy_flex_debug] [-l PROSOPON_LIBRARY]* [FILE...]
 *
 * OPTIONS
 * -yydebug
 *     Enables yydebug for bison.
 * -yy_flex_debug
 *     Enables yy_flex_debug for flex.
 * -l PROSOPON_LIBRARY
 *     Load a Prosopon library.
 */
int main(int argc, char** argv)
{
    cl_state cl = {
        .load_standard_library = 1
    };
 
    // set default options
    yydebug = 0;
    yy_flex_debug = 0;
    
    // process command line arguments
    pro_state_ref state;
    if (pro_state_create(&state) != PRO_OK)
        return 0;
    
    for (unsigned int i = 1; i < argc; ++i)
    {
        int status = process_args(&cl, state, argv[i]);
        if (status != 0)
            return -1;
    }
    
    // load standard library
    // //"/Users/mattbierner/Projects/prosopon/prosopon-stdlib/libprosopon-stdlib.so.1") != 0)

    if (cl.load_standard_library)
        if (process_library(state, "/Users/mattbierner/Projects/prosopon/build/Debug/libprosopon-stdlib.dylib"))
            return 0;
    
    // load libraries
    file_list* libraries = cl.libraries;
    for (const char* library = 0; library && (library = libraries->file); libraries = libraries->next)
    {
        int status = process_library(state, library);
        if (status != 0)
            return -1;
    }
    
    // process files
    file_list* files = cl.files;
    for (const char* file = 0; files && (file = files->file); files = files->next)
    {
        int status = process_file(state, file);
        if (status != 0)
            return -1;
    }
    
    pro_state_release(state);
    return 0;
}
