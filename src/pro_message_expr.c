#include "pro_message_expr.h"

#include <stdio.h>
#include <assert.h>

#include "pro_expr_list.h"


static void message_expr_eval(pro_state_ref s, pro_expr* t)
{
    assert(pro_expr_get_type(t) == PRO_MESSAGE_EXPR_TYPE);
    
    pro_ref msg;
    pro_message_create(s, &msg);
    
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
        }   break;
        default:
            pro_eval_expr(s, value);
            lookup = value->data.lookup;
            break;
        }
        pro_message_append(s, msg, lookup);
    }
    t->data.lookup = msg;
}

static void message_expr_print(pro_state_ref s, const pro_expr* t, const char* end)
{
    assert(pro_expr_get_type(t) == PRO_MESSAGE_EXPR_TYPE);
    printf("<message ");
    pro_print_expr_list(s, t->value.list, "");
    printf(">%s", end);
}


const pro_expr_type_info pro_message_expr_type_info = {
    .eval = message_expr_eval,
    .print = message_expr_print
};


PRO_INTERNAL pro_expr* pro_message_expr_create(pro_expr_list* list)
{
    pro_expr* t = pro_expr_create(PRO_MESSAGE_EXPR_TYPE);
    t->value.list = list;
    return t;
}
