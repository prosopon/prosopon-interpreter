#include "pro_actor_expr.h"

#include "pro_case_expr.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>


#pragma mark Private

static void actor_expr_eval(pro_state_ref s, pro_expr* t)
{
    assert(pro_expr_get_type(t) == PRO_ACTOR_EXPR_TYPE);
    pro_ref ref;
    pro_actor_create(s, 0, &ref);
    pro_expr* behavior_expr =  t->value.behavior;
    if (behavior_expr)
    {
        pro_behavior behavior = pro_actor_expr_get_behavior(s, t);
        pro_become(s, ref, behavior);
    }
    t->data.lookup = ref;
}

static void actor_expr_print(pro_state_ref s, const pro_expr* t, const char* end)
{
    assert(pro_expr_get_type(t) == PRO_ACTOR_EXPR_TYPE);
    pro_expr* behavior =  t->value.behavior;
    
    printf("<actor behavior:");
    pro_print_expr(s, behavior, "");
    printf(">%s", end);
}

static void actor_expr_release(pro_expr* t)
{
    pro_release_expr(t->value.behavior);
    free(t);
}

static void behavior(pro_state_ref s,
    pro_ref t, pro_ref msg, void* data)
{
    pro_expr* behavior_expr = data;
    pro_expr_list* case_list = behavior_expr->value.list;
        
    while (case_list)
    {
        pro_expr* case_expr = case_list->value;
        if (pro_case_expr_match(s, case_expr, msg) != 0)
            return;
        case_list = case_list->next;
    }
}



#pragma mark -
#pragma mark Internal 

const pro_expr_type_info pro_actor_expr_type_info = {
    .eval = actor_expr_eval,
    .print = actor_expr_print,
    .release = actor_expr_release
};


PRO_INTERNAL pro_expr* pro_actor_expr_create(pro_expr* behavior)
{
    pro_expr* t = pro_expr_create(PRO_ACTOR_EXPR_TYPE);
    t->value.behavior = behavior;
    return t;
}


PRO_INTERNAL pro_behavior pro_actor_expr_get_behavior(pro_state_ref s, 
    pro_expr* t)
{
    assert(pro_expr_get_type(t) == PRO_ACTOR_EXPR_TYPE);
    pro_expr* behavior_expr = t->value.behavior;
    pro_behavior beh = {
        .impl = behavior,
        .data = behavior_expr
    };
    return beh;
}

