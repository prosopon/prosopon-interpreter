#ifndef prosopon_pro_expr_type_h
#define prosopon_pro_expr_type_h

#include "prosopon.h"

struct pro_expr;


/**
 * An enum to identify the type of an expression.
 */
typedef enum
{
    PRO_NONE_EXPR_TYPE      = -1,
    PRO_LIST_EXPR_TYPE      = 0,
    PRO_LET_EXPR_TYPE,
    PRO_SEND_EXPR_TYPE,
    PRO_BECOME_EXPR_TYPE,
    PRO_CASE_EXPR_TYPE,
    PRO_STRING_EXPR_TYPE,
    PRO_NUMBER_EXPR_TYPE,
    PRO_CAPTURE_IDENTIFIER_EXPR_TYPE,
    PRO_IDENTIFIER_EXPR_TYPE,
    PRO_MESSAGE_EXPR_TYPE,
    PRO_ACTOR_EXPR_TYPE,
    PRO_CONSTRUCTOR_EXPR_TYPE,
    PRO_MAX_EXPR_TYPE
} pro_expr_type;


/**
 * A function that evaluates an expression. 
 */
typedef pro_ref(pro_expr_eval)(pro_state_ref, struct pro_expr* t);

/**
 * Print out the human readable representation of an expression.
 *
 * @param t This, the expression to print.
 * @param end A string to append the end of the printing.
 */
typedef void(pro_expr_print)(pro_state_ref,
    const struct pro_expr* t, const char* end);

/**
 * A structure to hold information on a given type.
 */
typedef struct 
{
    pro_expr_eval* const eval;
    pro_expr_print* const print;
    pro_ud_deconstructor* release;
} pro_expr_type_info;


/**
 * An lookup table for info about expression types.
 */
extern const pro_expr_type_info* pro_types[PRO_MAX_EXPR_TYPE];



#endif
