#include "pro_case_expr.h"

#include <stdio.h>
#include <assert.h>


static void case_expr_eval(pro_state_ref s, pro_expr* t)
{
    assert(0);
}


static void case_expr_print(pro_state_ref s, const pro_expr* t, const char* end)
{
    assert(pro_expr_get_type(t) == PRO_CASE_EXPR_TYPE);
    
    pro_expr* pattern = t->value.binary.left;
    pro_expr* body = t->value.binary.right;
    
    printf("<case pattern:");
    pro_print_expr(s, pattern, " body:");
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


PRO_INTERNAL int pro_case_expr_match(pro_state_ref s,
    pro_expr* t, pro_ref msg)
{
    pro_env_ref current_env, env;
    pro_get_env(s, &current_env);
    pro_env_create(s, current_env, &env);
    pro_push_env(s, env);
    
    pro_expr* pattern = t->value.binary.left;
    pro_expr* body = t->value.binary.right;

    unsigned int msg_length;
    pro_message_length(s, msg, &msg_length);
    
    pro_expr_list* match_list = pattern->value.list;
    for (unsigned int index = 0; match_list; ++index)
    {
        if (index >= msg_length)
        {
            pro_pop_env(s);
            return 0;
        }
        pro_ref arg;
        pro_message_get(s, msg, index, &arg);
        pro_expr* match = match_list->value;
        
        switch (pro_expr_get_type(match))
        {
        case PRO_IDENTIFIER_EXPR_TYPE:
            pro_bind(s, arg, match->value.identifier);
            break;
        default:
            pro_eval_expr(s, match);
            if (!pro_match(s, arg, match->data.lookup))
            {
                pro_pop_env(s);
                return 0;
            }
            break;
        }
        match_list = match_list->next;
    }
    
    pro_eval_expr(s, body);
    pro_pop_env(s);
    return 1;
}


