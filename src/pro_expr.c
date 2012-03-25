#include "pro_expr.h"

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>


PRO_INTERNAL pro_expr* pro_expr_create(pro_expr_type type)
{
    pro_expr* t = malloc(sizeof(*t));
    t->type = type;
    return t;
}


PRO_INTERNAL void pro_print_expr(pro_state* s, const pro_expr* t, const char* end)
{
    assert(t);
    const pro_expr_type type = pro_expr_get_type(t);
    pro_types[type]->print(s, t, end);
}


PRO_INTERNAL pro_expr_type pro_expr_get_type(const pro_expr* t)
{
    assert(t);
    return t->type;
}


PRO_INTERNAL void pro_eval_expr(pro_state* s, pro_expr* t)
{
    assert(t);
    const pro_expr_type type = pro_expr_get_type(t);
    pro_types[type]->eval(s, t);
}
