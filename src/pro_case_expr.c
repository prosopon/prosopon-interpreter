#include "pro_case_expr.h"

#include "prosopon_macros.h"

#include <stdio.h>
#include <assert.h>


static pro_ref case_expr_eval(pro_state_ref s, pro_ref ref, pro_expr* t)
{
    assert(0);
    return PRO_EMPTY_REF;
}


static void case_expr_print(pro_state_ref s, const pro_expr* t, const char* end)
{
    assert(pro_expr_get_type(t) == PRO_CASE_EXPR_TYPE);
    
    pro_ref pattern = t->value.binary.left;
    pro_ref body = t->value.binary.right;
    
    printf("<case pattern:");
    if (pattern)
        pro_print_expr(s, pattern, " body:");
    if (body)
        pro_print_expr(s, body, "");
    printf(">%s", end);
}


static void case_expr_release(pro_state_ref s, void* data)
{
    pro_expr* t = data;
    
    pro_ref pattern = t->value.binary.left;
    pro_ref body = t->value.binary.right;
    if (pattern)
        pro_release(s, pattern);
    if (body)
        pro_release(s, body);
        
    PRO_DEFAULT_UD_DECONSTRUCTOR(s, data);
}


#pragma mark -
#pragma mark Internal

const pro_expr_type_info pro_case_expr_type_info = {
    .eval = case_expr_eval,
    .print = case_expr_print,
};


PRO_INTERNAL pro_ref pro_case_expr_create(pro_state_ref s,
    pro_ref pattern, pro_ref body)
{
    pro_expr* t;
    pro_ref ref = pro_expr_create(s, PRO_CASE_EXPR_TYPE, case_expr_release, &t);
    if (!t) return 0;
    t->value.binary.left = pattern;
    t->value.binary.right = body;
    return ref;
}


PRO_INTERNAL int pro_case_expr_match(pro_state_ref s,
    pro_expr* t, pro_ref msg)
{
    // Setup the environment for matching.
    {
        pro_env_ref current_env, env;
        pro_get_env(s, &current_env);
        pro_env_create(s, current_env, &env);
        pro_env_release(s, current_env);
        pro_push_env(s, env);
        pro_env_release(s, env);
    }
    
    pro_ref body_ref = t->value.binary.right;
    pro_ref pattern_ref = t->value.binary.left;

    pro_expr* body;
    pro_expr* pattern;
    if (body_ref)
        pro_ud_write(s, body_ref, (void**)&body);
    else
        body = 0;
    
    if (pattern_ref)
        pro_ud_write(s, pattern_ref, (void**)&pattern);
    else
        pattern = 0;

    unsigned int msg_length;
    pro_list_length(s, msg, &msg_length);
    
    pro_expr_list* match_list = pattern->value.list;
    for (unsigned int index = 0; index < msg_length; ++index)
    {
        pro_ref arg;
        pro_list_get(s, msg, index, &arg);
        
        pro_ref match_ref = match_list->value;
        pro_ref match_val;
        if (pro_match_type(s, match_ref, PRO_UD_TYPE))
            match_val = pro_eval_expr(s, match_ref);
        else
        {
            pro_retain(s, match_ref);
            match_val = match_ref;
        }
            
        pro_matching do_match;
        pro_match(s, match_val, arg, &do_match);
        pro_release(s, match_val);
        switch (do_match)
        {
        case PRO_MATCH_FAIL:
            pro_release(s, arg);
            pro_pop_env(s);
            return 0;
        case PRO_MATCH_CONTINUE:
            pro_release(s, arg);
            continue;
        }
        
        pro_release(s, arg);
        
        if (!(match_list = match_list->next) && (index + 1) != msg_length)
        {
            pro_pop_env(s);
            return 0;
        }
    }
    
    if (body)
    {
        pro_ref out = pro_eval_expr(s, body_ref);
        pro_release(s, out);
    }
    
    pro_pop_env(s);
    
    return 1;
}


