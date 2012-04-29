#include "pro_number_expr.h"

#include <prosopon/prosopon_stdlib.h>

#include <stdio.h>
#include <assert.h>


#pragma mark Private

static pro_ref number_expr_eval(pro_state_ref s, pro_ref ref, pro_expr* t)
{
    assert(pro_expr_get_type(t) == PRO_NUMBER_EXPR_TYPE);
    ///pro_retain(s, ref);
    //return ref;
  return pro_number_create(s, t->value.number);
}


static void number_expr_print(pro_state_ref s, const pro_expr* t, const char* end)
{
    assert(pro_expr_get_type(t) == PRO_NUMBER_EXPR_TYPE);
    double value = t->value.number;
    printf("<number %f>%s", value, end);
}


#pragma mark -
#pragma mark Internal 

const pro_expr_type_info pro_number_expr_type_info = {
    .eval = number_expr_eval,
    .print = number_expr_print,
};


PRO_INTERNAL
pro_ref pro_number_expr_create(pro_state_ref s, double value)
{
    pro_expr* t;
    pro_ref ref = pro_expr_create(s, PRO_NUMBER_EXPR_TYPE,PRO_DEFAULT_UD_DECONSTRUCTOR,  &t);
    t->value.number = value;
    return ref;

  //  pro_ref r = pro_number_create(s, value);
  //  return r;
}
