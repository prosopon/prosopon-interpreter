#include "pro_actor_expr.h"

#include <assert.h>
#include <stdio.h>


static void actor_expr_eval(pro_state* s, pro_expr* t)
{
    assert(pro_expr_get_type(t) == PRO_ACTOR_EXPR_TYPE);
}

static void actor_expr_print(pro_state* s, pro_expr* t, const char* end)
{
    assert(pro_expr_get_type(t) == PRO_ACTOR_EXPR_TYPE);
    pro_expr* behavior =  t->value.behavior;
    
    printf("<actor behavior:");
    pro_print_expr(s, behavior, "");
    printf(">%s", end);
}

const pro_expr_type_info pro_actor_expr_type_info = {
    .eval = actor_expr_eval,
    .print = actor_expr_print
};


PRO_INTERNAL pro_expr* pro_actor_expr_create(pro_expr* behavior)
{
    pro_expr* t = pro_expr_create(PRO_ACTOR_EXPR_TYPE);
    t->value.behavior = behavior;
    return t;
}


PRO_INTERNAL pro_behavior* pro_actor_expr_get_behavior(pro_state* s, 
    pro_expr* t, void** data)
{
    assert(pro_expr_get_type(t) == PRO_ACTOR_EXPR_TYPE);
    
    return 0;
}