#include "pro_become_expr.h"

#include <assert.h>
#include <stdio.h>


#pragma mark Private

static pro_ref become_expr_eval(pro_state_ref s, pro_ref ref, pro_expr* t)
{
    assert(pro_expr_get_type(t) == PRO_BECOME_EXPR_TYPE);
    
    pro_ref left = t->value.binary.left;
    pro_ref right = t->value.binary.right;
    
    pro_ref left_result = pro_eval_expr(s, left);
    pro_ref right_result = pro_eval_expr(s, right);
    pro_become(s, left_result, right_result);

    pro_release(s, left_result);
    pro_release(s, right_result);
    
    return PRO_EMPTY_REF;
}

static void become_expr_print(pro_state_ref s, const pro_expr* t, const char* end)
{
    pro_ref left = t->value.binary.left;
    pro_ref right = t->value.binary.right;

    printf("<Become id:");
    pro_print_expr(s, left, " ");
    printf("value:");
    pro_print_expr(s, right, "");
    printf(">%s", end);
}

static void become_expr_release(pro_state_ref s, void* data)
{
    pro_expr* t = data;
    pro_release(s, t->value.binary.left);
    pro_release(s, t->value.binary.right);
    PRO_DEFAULT_UD_DECONSTRUCTOR(s, data);
}


#pragma mark -
#pragma mark Internal

const pro_expr_type_info pro_become_expr_type_info = {
    .eval = become_expr_eval,
    .print = become_expr_print,
};


PRO_INTERNAL pro_ref pro_become_expr_create(pro_state_ref s,
    pro_ref identifier, pro_ref value)
{
    pro_expr* t;
    pro_ref ref = pro_expr_create(s, PRO_BECOME_EXPR_TYPE, become_expr_release, &t);
    t->value.binary.left = identifier;
    t->value.binary.right = value;
    return ref;
}
