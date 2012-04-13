#include "pro_number_expr.h"

#include "prosopon_stdlib.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>


#pragma mark Private

static pro_ref number_expr_eval(pro_state_ref s, pro_expr* t)
{
    assert(pro_expr_get_type(t) == PRO_NUMBER_EXPR_TYPE);
    return pro_number_create(s, t->value.number);
}


static void number_expr_print(pro_state_ref s, const pro_expr* t, const char* end)
{
    assert(pro_expr_get_type(t) == PRO_NUMBER_EXPR_TYPE);
    double value = t->value.number;
    printf("<number %f>%s", value, end);
}


static void number_expr_release(pro_expr* t)
{
    free(t);
}


#pragma mark -
#pragma mark Internal 

const pro_expr_type_info pro_number_expr_type_info = {
    .eval = number_expr_eval,
    .print = number_expr_print,
    .release = number_expr_release
};


PRO_INTERNAL pro_expr* pro_number_expr_create(double value)
{
    pro_expr* t = pro_expr_create(PRO_NUMBER_EXPR_TYPE);
    t->value.number = value;
    return t;
}
