#include "pro_message_expr.h"

#include "pro_expr_list.h"

#include <prosopon/prosopon_macros.h>

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>


static pro_ref message_expr_eval(pro_state_ref s, pro_ref ref, pro_expr* t)
{
    assert(pro_expr_get_type(t) == PRO_MESSAGE_EXPR_TYPE);
    
    pro_ref msg;
    pro_list_create(s, &msg);
    
    for (pro_expr_list* list = t->value.list; list; list = list->next)
    {
        pro_ref value_ref = list->value;
        pro_ref lookup = 0;

        if (pro_match_type(s, value_ref, PRO_UD_TYPE))
        {
            pro_expr* value;
            pro_ud_write(s, value_ref, (void **)&value);
        
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
                lookup = pro_eval_expr(s, value_ref);
                break;
            }
        }
        else
        {
            pro_retain(s, value_ref);
            lookup = value_ref;
        }

        pro_list_append_inplace(s, &msg, lookup);
        pro_release(s, lookup);
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


static void message_expr_release(pro_state_ref s, void* data)
{
    pro_expr* t = data;
    pro_release_expr_list(s, t->value.list);
    PRO_DEFAULT_UD_DECONSTRUCTOR(s, data);
}


#pragma mark -
#pragma mark Internal 

const pro_expr_type_info pro_message_expr_type_info = {
    .eval = message_expr_eval,
    .print = message_expr_print,
};


PRO_INTERNAL
pro_ref pro_message_expr_create(pro_state_ref s,
    pro_ref list_ref)
{    
    pro_expr* t;
    pro_ref ref = pro_expr_create(s, PRO_MESSAGE_EXPR_TYPE, message_expr_release, &t);
    
    if (list_ref)
    {
        pro_expr* list;
        pro_ud_write(s, list_ref, (void**)&list);
        t->value.list = list->value.list;
        list->value.list = 0;
    }
    else
        t->value.list = 0;
        
    return ref;
}
