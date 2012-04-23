#ifndef prosopon_interpreter_prosopon_interpreter_h
#define prosopon_interpreter_prosopon_interpreter_h

#include "prosopon.h"


typedef struct pro_interpreter_state pro_interpreter_state;

PRO_INTERPRETER pro_interpreter_state* (pro_interpreter_state_create) (pro_alloc*);
PRO_INTERPRETER void (pro_interpreter_state_release) (pro_interpreter_state*);

PRO_INTERPRETER pro_alloc* (pro_interpreter_state_get_alloc) (pro_interpreter_state*);


PRO_INTERPRETER void (pro_interpreter_state_set_debug) (pro_interpreter_state*, int);
PRO_INTERPRETER void (pro_interpreter_state_set_yacc_debug) (pro_interpreter_state*, int);
PRO_INTERPRETER void (pro_interpreter_state_set_flex_debug) (pro_interpreter_state*, int);


#pragma mark -
#pragma mark API

/**
 * Evaluate a given file using the prosopon interpreter.
 */
PRO_INTERPRETER int (pro_eval) (pro_interpreter_state*, const char* file);

PRO_INTERPRETER int (pro_process_library) (pro_interpreter_state*, const char* file);

PRO_INTERPRETER int (load_stdlib_library) (pro_interpreter_state*, const char* path);

#endif
