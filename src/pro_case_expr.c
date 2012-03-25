#include "pro_case_expr.h"

#include <stdio.h>
#include <assert.h>


static void case_expr_eval(pro_state* s, pro_expr* t)
{
    assert(pro_expr_get_type(t) == PRO_CASE_EXPR_TYPE);
}


static void case_expr_print(pro_state* s, const pro_expr* t, const char* end)
{
    assert(pro_expr_get_type(t) == PRO_CASE_EXPR_TYPE);
    
    pro_expr* pattern = t->value.binary.left;
    pro_expr* body = t->value.binary.right;
    
    printf("<case pattern:");
    pro_print_expr(s, pattern, " ");
    printf("body:");
    pro_print_expr(s, body, "");
    printf(">%s", end);
}

const pro_expr_type_info pro_case_expr_type_info = {
    .eval = case_expr_eval,
    .print = case_expr_print
};


PRO_INTERNAL pro_expr* pro_case_expr_create(pro_expr* pattern, pro_expr* body)
{
    pro_expr* t = pro_expr_create(PRO_CASE_EXPR_TYPE);
    t->value.binary.left = pattern;
    t->value.binary.right = body;
    return t;
}
