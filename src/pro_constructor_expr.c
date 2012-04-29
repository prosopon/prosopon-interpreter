#include "pro_constructor_expr.h"

#include "pro_expr.h"
#include "pro_expr_list.h"

#include <prosopon/prosopon_macros.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>


static pro_ref constructor_expr_eval(pro_state_ref s, pro_ref ref, pro_expr* t)
{
    assert(pro_expr_get_type(t) == PRO_CONSTRUCTOR_EXPR_TYPE);
    
    char* identifier = t->value.constructor.identifier;
    
    pro_alloc* alloc;
    pro_get_alloc(s, &alloc);
    
    // lookup the constructor
    pro_env_ref env;
    pro_get_env(s, &env);
    pro_ref constructor;
    pro_get_binding(s, env, identifier, &constructor);
    pro_env_release(s, env);
    
    if (0 == constructor)
        return PRO_EMPTY_REF;
    
    // build the list of arguments
    pro_ref arguments =  t->value.constructor.arguments;
    pro_expr* arguments_expr = 0;
    pro_expr_list* expr_arg_list = 0;
    if (arguments)
    {
        pro_ud_write(s, arguments, (void**)&arguments_expr);
        expr_arg_list = arguments_expr->value.list;
    }

    pro_ref arg_list;
    pro_list_create(s, &arg_list);

    for (pro_expr_list* expr_arg = expr_arg_list; expr_arg; expr_arg = expr_arg->next)
    {
        pro_ref value = expr_arg->value;
        if (value)
        {
            pro_ref lookup = pro_eval_expr(s, value);
            pro_list_append_inplace(s, &arg_list, lookup);
            pro_release(s, lookup);
        }
    }
    
    // Call the constructor
    pro_ref out;
    pro_constructor_call(s, constructor, arg_list, &out);
    pro_release(s, constructor);
    
    pro_release(s, arg_list);
    
    return out;
}

static void constructor_expr_print(pro_state_ref s,
    const pro_expr* t, const char* end)
{
    assert(pro_expr_get_type(t) == PRO_CONSTRUCTOR_EXPR_TYPE);
    
    printf("<constructor %s arguments:", t->value.constructor.identifier);
    pro_print_expr_list(s, t->value.constructor.arguments, "");
    printf(">%s", end);
}


static void constructor_expr_release(pro_state_ref s, void* data)
{
    pro_expr* t = data;
    pro_alloc* alloc;
    pro_get_alloc(s, &alloc);
    alloc(t->value.constructor.identifier, 0);
    pro_release(s, t->value.constructor.arguments);
    PRO_DEFAULT_UD_DECONSTRUCTOR(s, data);
}

const pro_expr_type_info pro_constructor_expr_type_info = {
    .eval = constructor_expr_eval,
    .print = constructor_expr_print,
};


PRO_INTERNAL
pro_ref pro_constructor_expr_create(pro_state_ref s,
    char* identifier, pro_ref arguments_ref)
{
    pro_expr* t;
    pro_ref ref = pro_expr_create(s, PRO_CONSTRUCTOR_EXPR_TYPE, constructor_expr_release, &t);
    t->value.constructor.identifier = identifier;
    t->value.constructor.arguments = arguments_ref;
    return ref;
}
