#include "pro_actor_expr.h"

#include "pro_case_expr.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>


#pragma mark Private

static pro_ref actor_expr_eval(pro_state_ref s, pro_expr* t)
{
    assert(pro_expr_get_type(t) == PRO_ACTOR_EXPR_TYPE);
    pro_ref behavior_expr =  t->value.behavior;
    
    pro_behavior* behavior = 0;
    pro_ref ud = 0;
    if (behavior_expr)
         behavior = pro_actor_expr_get_behavior(s, t, &ud);
        
    pro_ref ref;
    pro_actor_create(s, PRO_DEFAULT_ACTOR_TYPE, behavior, ud, &ref);
    pro_release(s, ud);
    return ref;
}

static void actor_expr_print(pro_state_ref s, const pro_expr* t, const char* end)
{
    assert(pro_expr_get_type(t) == PRO_ACTOR_EXPR_TYPE);
    pro_expr* behavior =  t->value.behavior;
    
    printf("<actor behavior:");
    if (behavior)
        pro_print_expr(s, behavior, "");
    printf(">%s", end);
}

static void actor_expr_release(pro_state_ref s, void* data)
{
    pro_expr* t = data;
    if (t->value.behavior)
        pro_release(s, t->value.behavior);
        
    PRO_DEFAULT_UD_DECONSTRUCTOR(s, data);
}

static void behavior(pro_state_ref s,
    pro_ref t, pro_ref msg, pro_ref data)
{
    // setup environement
    pro_bind(s, t, "self"); // setup self
    pro_bind(s, msg, "msg"); // setup msg

    pro_ref* behavior_expr;
    pro_ud_write(s, data, (void**)&behavior_expr);
    
    pro_expr* behavior_val;
    pro_ud_write(s, *behavior_expr, (void**)&behavior_val);
    
    pro_expr_list* case_list = behavior_val->value.list;
    while (case_list)
    {
        pro_ref case_expr = case_list->value;
        pro_expr* case_expr_data;
        pro_ud_write(s, case_expr, (void**)&case_expr_data);
        if (pro_case_expr_match(s, case_expr_data, msg) != 0)
            break;
        case_list = case_list->next;
    }
}

static void behavior_deconstructor(pro_state_ref s, void* data)
{
    pro_alloc* alloc;
    pro_get_alloc(s, &alloc);
    
    pro_expr** expr = data;
    
   // pro_release(*expr);
    alloc(expr, 0);
}



#pragma mark -
#pragma mark Internal 

const pro_expr_type_info pro_actor_expr_type_info = {
    .eval = actor_expr_eval,
    .print = actor_expr_print,
    .release = actor_expr_release
};


PRO_INTERNAL pro_ref pro_actor_expr_create(pro_state_ref s,
    pro_ref behavior)
{
    pro_expr* t;
    pro_ref ref = pro_expr_create(s, PRO_ACTOR_EXPR_TYPE, &t);
    t->value.behavior = behavior;
    return ref;
}


PRO_INTERNAL pro_behavior* pro_actor_expr_get_behavior(pro_state_ref s, 
    pro_expr* t, pro_ref* ud)
{
    assert(pro_expr_get_type(t) == PRO_ACTOR_EXPR_TYPE);
    pro_ref behavior_expr = t->value.behavior;

    pro_ud_create(s, sizeof(t), behavior_deconstructor, ud);
    
    pro_ref* expr_data;
    pro_ud_write(s, *ud, (void**)&expr_data);
    *expr_data = behavior_expr;
    
    return behavior;
}

