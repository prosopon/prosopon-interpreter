#include "pro_constructor_expr.h"

#include "pro_expr_list.h"

#include <assert.h>
#include <stdio.h>


static void constructor_expr_eval(pro_state* s, pro_expr* t)
{
    assert(pro_expr_get_type(t) == PRO_CONSTRUCTOR_EXPR_TYPE);
    
}

static void constructor_expr_print(pro_state* s, pro_expr* t, const char* end)
{
    assert(pro_expr_get_type(t) == PRO_CONSTRUCTOR_EXPR_TYPE);
    
    printf("<constructor %s list arguments:", t->value.constructor.identifier);
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
