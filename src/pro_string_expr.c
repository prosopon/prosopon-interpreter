#include "pro_string_expr.h"

#include <assert.h>
#include <stdio.h>


static void string_expr_eval(pro_state* s, pro_expr* t)
{
    assert(pro_expr_get_type(s, t) == PRO_STRING_EXPR_TYPE);
}

static void string_expr_print(pro_state* s, pro_expr* t)
{
    assert(pro_expr_get_type(s, t) == PRO_STRING_EXPR_TYPE);
    char* value = t->value.string;
    printf("<string %s>\n", value);
}

const pro_expr_type_info pro_string_expr_type_info = {
    .eval = string_expr_eval,
    .print = string_expr_print
};


PRO_INTERNAL pro_expr* pro_string_expr_create(pro_state* s, char* value)
{
    pro_expr* t = pro_expr_create(s, PRO_STRING_EXPR_TYPE);
    t->value.string = value;
    return t;
}
