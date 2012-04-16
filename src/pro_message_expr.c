#include "pro_message_expr.h"

#include "pro_expr_list.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>


static pro_ref message_expr_eval(pro_state_ref s, pro_expr* t)
{
    assert(pro_expr_get_type(t) == PRO_MESSAGE_EXPR_TYPE);
    
    pro_ref msg;
    pro_list_create(s, &msg);
    
    for (pro_expr_list* list = t->value.list; list; list = list->next)
    {
        pro_expr* value = list->value;
        pro_ref lookup = 0;
        
        switch (pro_expr_get_type(value))
        {
        case PRO_IDENTIFIER_EXPR_TYPE:
        {
            pro_env_ref env;
            pro_get_env(s, &env);
            pro_get_binding(s, env, value->value.identifier, &lookup);
            pro_env_release(s, env);
        }   break;
        default:
            lookup = pro_eval_expr(s, value);
            break;
        }
        
        pro_ref new_msg = 0;
        pro_list_append(s, msg, lookup, &new_msg);
        pro_release(s, msg);
        pro_release(s, lookup);

        msg = new_msg;
    }
    return msg;
}


static void message_expr_print(pro_state_ref s, const pro_expr* t, const char* end)
{
    assert(pro_expr_get_type(t) == PRO_MESSAGE_EXPR_TYPE);
    printf("<message ");
    
    if (t->value.list)
        pro_print_expr_list(s, t->value.list, "");
    printf(">%s", end);
}


static void message_expr_release(pro_state_ref s, pro_expr* t)
{
    pro_alloc* alloc;
    pro_get_alloc(s, &alloc);

    pro_release_expr_list(s, t->value.list);
    alloc(t, 0);
}


#pragma mark -
#pragma mark Internal 

const pro_expr_type_info pro_message_expr_type_info = {
    .eval = message_expr_eval,
    .print = message_expr_print,
    .release = message_expr_release
};


PRO_INTERNAL pro_expr* pro_message_expr_create(pro_state_ref s,
    pro_expr_list* list)
{
    pro_expr* t = pro_expr_create(s, PRO_MESSAGE_EXPR_TYPE);
    t->value.list = list;
    return t;
}
