#ifndef prosopon_pro_expr_h
#define prosopon_pro_expr_h

#include "prosopon.h"
#include "pro_expr_type.h"


typedef struct pro_expr pro_expr;
typedef struct pro_expr_list pro_expr_list;


struct pro_expr_list
{
    pro_expr* value;
    pro_expr_list* next;
};

// Based on http://lambda.uta.edu/cse5317/notes/node25.html
struct pro_expr
{
    pro_expr_type type;
    union
    {
        double number;
        char* identifier;
        char* string;
        struct
        {
            char* identifier;
            pro_expr* left;
            pro_expr* right;
        } binary;
        struct
        {
            char* identifier;
            pro_expr_list* arguments;
        } constructor;
        pro_expr_list* list;
        pro_expr* behavior;
    } value;
};


/**
 * @return A new exprssion of a given type.
 */
PRO_INTERNAL pro_expr* pro_expr_create(pro_expr_type type);

/**
 * Evaluates a given expression.
 */
PRO_INTERNAL pro_ref pro_eval_expr(pro_state_ref, pro_expr*);

/**
 * Prints a given expression
 */
PRO_INTERNAL void pro_print_expr(pro_state_ref, const pro_expr* t,
    const char* end);
 
/**
 * Releases a given expression for memory collection.
 */
PRO_INTERNAL void pro_release_expr(pro_expr* t);


/**
 * @returns The type identifier of a given expression.
 */
PRO_INTERNAL pro_expr_type pro_expr_get_type(const pro_expr*);



#endif
