#include "pro_string_expr.h"

#include "prosopon_libcore.h"

#include <assert.h>
#include <stdio.h>


static void string_expr_eval(pro_state* s, pro_expr* t)
{
    assert(pro_expr_get_type(t) == PRO_STRING_EXPR_TYPE);
    t->data.lookup = pro_string_create(s, t->value.string);
}

static void string_expr_print(pro_state* s, const pro_expr* t, const char* end)
{
    assert(pro_expr_get_type(t) == PRO_STRING_EXPR_TYPE);
    const char* value = t->value.string;
    printf("<string %s>%s", value, end);
}

const pro_expr_type_info pro_string_expr_type_info = {
    .eval = string_expr_eval,
    .print = string_expr_print
};


PRO_INTERNAL pro_expr* pro_string_expr_create(char* value)
{
    pro_expr* t = pro_expr_create(PRO_STRING_EXPR_TYPE);
    t->value.string = value;
    return t;
}
