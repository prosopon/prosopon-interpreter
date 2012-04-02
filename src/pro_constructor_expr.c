#include "pro_constructor_expr.h"

#include "pro_expr.h"
#include "pro_expr_list.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>


static void constructor_expr_eval(pro_state* s, pro_expr* t)
{
    assert(pro_expr_get_type(t) == PRO_CONSTRUCTOR_EXPR_TYPE);
    
    char* identifier = t->value.constructor.identifier;
    
    // lookup the constructor 
    pro_lookup* constructor = pro_get_binding(s, pro_get_env(s), identifier);
    if (0 == constructor)
        return;
    
    // build the list of arguments
    pro_lookup_list* arg_list = malloc(sizeof(*arg_list));
    pro_lookup_list* arg = arg_list;
    pro_expr_list* expr_arg_list = t->value.constructor.arguments;
    pro_expr_list* expr_arg = expr_arg_list;
    while (expr_arg)
    {
        pro_expr* value = expr_arg->value;
        if (value)
        {
            pro_eval_expr(s, value);
            arg->value = value->data.lookup;
            arg->next = malloc(sizeof(*arg));
        }
                
        expr_arg = expr_arg->next;
        arg = arg->next;
    }
    
    // Call the constructor
    t->data.lookup = pro_constructor_call(s, constructor, arg_list);
}

static void constructor_expr_print(pro_state* s,
    const pro_expr* t, const char* end)
{
    assert(pro_expr_get_type(t) == PRO_CONSTRUCTOR_EXPR_TYPE);
    
    printf("<constructor %s arguments:", t->value.constructor.identifier);
    pro_print_expr_list(s, t->value.constructor.arguments, "");
    printf(">%s", end);
}

const pro_expr_type_info pro_constructor_expr_type_info = {
    .eval = constructor_expr_eval,
    .print = constructor_expr_print
};


PRO_INTERNAL pro_expr* pro_constructor_expr_create(
    char* identifier, pro_expr_list* arguments)
{
    pro_expr* t = pro_expr_create(PRO_CONSTRUCTOR_EXPR_TYPE);
    t->value.constructor.identifier = identifier;
    t->value.constructor.arguments = arguments;
    return t;
}
