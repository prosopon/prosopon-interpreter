#include "pro_become_expr.h"

#include <assert.h>
#include <stdio.h>


#pragma mark Private

static pro_ref become_expr_eval(pro_state_ref s, pro_expr* t)
{
    assert(pro_expr_get_type(t) == PRO_BECOME_EXPR_TYPE);
    
    pro_expr* left = t->value.binary.left;
    pro_expr* right = t->value.binary.right;
    pro_become(s, pro_eval_expr(s, left), pro_eval_expr(s, right));
    return PRO_EMPTY_REF;
}

static void become_expr_print(pro_state_ref s, const pro_expr* t, const char* end)
{
    pro_expr* left = t->value.binary.left;
    pro_expr* right = t->value.binary.right;

    printf("<Become id:");
    pro_print_expr(s, left, " ");
    printf("value:");
    pro_print_expr(s, right, "");
    printf(">%s", end);
}

static void become_expr_release(pro_state_ref s, pro_expr* t)
{
    pro_alloc* alloc;
    pro_get_alloc(s, &alloc);
    pro_release_expr(s, t->value.binary.left);
    pro_release_expr(s, t->value.binary.right);
    alloc(t, 0);
}



#pragma mark -
#pragma mark Internal

const pro_expr_type_info pro_become_expr_type_info = {
    .eval = become_expr_eval,
    .print = become_expr_print,
    .release = become_expr_release
};


PRO_INTERNAL pro_expr* pro_become_expr_create(pro_state_ref s,
    pro_expr* identifier, pro_expr* value)
{
    pro_expr* t = pro_expr_create(s, PRO_BECOME_EXPR_TYPE);
    t->value.binary.left = identifier;
    t->value.binary.right = value;
    return t;
}
