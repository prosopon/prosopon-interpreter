#include "pro_string_expr.h"

#include "prosopon_libcore.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>


#pragma mark Private

static pro_ref string_expr_eval(pro_state_ref s, pro_expr* t)
{
    assert(pro_expr_get_type(t) == PRO_STRING_EXPR_TYPE);
    return pro_string_create(s, t->value.string);
}

static void string_expr_print(pro_state_ref s, const pro_expr* t, const char* end)
{
    assert(pro_expr_get_type(t) == PRO_STRING_EXPR_TYPE);
    const char* value = t->value.string;
    printf("<string %s>%s", value, end);
}

static void string_expr_release(pro_expr* t)
{
    free(t->value.string);
    free(t);
}


#pragma mark -
#pragma mark Internal

const pro_expr_type_info pro_string_expr_type_info = {
    .eval = string_expr_eval,
    .print = string_expr_print,
    .release = string_expr_release
};


PRO_INTERNAL pro_expr* pro_string_expr_create(char* value)
{
    pro_expr* t = pro_expr_create(PRO_STRING_EXPR_TYPE);
    t->value.string = value;
    return t;
}
