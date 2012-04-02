#include "pro_identifier_expr.h"

#include <assert.h>
#include <stdio.h>


static void identifier_expr_eval(pro_state* s, pro_expr* t)
{
    assert(pro_expr_get_type(t) == PRO_IDENTIFIER_EXPR_TYPE);
    
    t->data.lookup = pro_get_binding(s, pro_get_env(s), t->value.identifier);
}

static void identifier_expr_print(pro_state* s,
    const pro_expr* t, const char* end)
{
    assert(pro_expr_get_type(t) == PRO_IDENTIFIER_EXPR_TYPE);
    char* value = t->value.identifier;
    
    printf("<identifier %s>\n", value);
}

const pro_expr_type_info pro_identifier_expr_type_info = {
    .eval = identifier_expr_eval,
    .print = identifier_expr_print
};


PRO_INTERNAL pro_expr* pro_identifier_expr_create(char* value)
{
    pro_expr* t = pro_expr_create(PRO_IDENTIFIER_EXPR_TYPE);
    t->value.identifier = value;
    return t;
}
