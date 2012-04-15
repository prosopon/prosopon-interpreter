#include "pro_constructor_expr.h"

#include "pro_expr.h"
#include "pro_expr_list.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>


static pro_ref constructor_expr_eval(pro_state_ref s, pro_expr* t)
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
    pro_expr_list* expr_arg_list = t->value.constructor.arguments;

    pro_ref_list arg_list = expr_arg_list ? alloc(0, sizeof(*arg_list)) : 0;
    arg_list->value = PRO_EMPTY_REF;
    pro_ref_list arg = arg_list;
    
    for (pro_expr_list* expr_arg = expr_arg_list; expr_arg; expr_arg = expr_arg->next, arg = arg->next)
    {
        pro_expr* value = expr_arg->value;
        if (value)
        {
            arg->value = pro_eval_expr(s, value);
            if (expr_arg->next)
                arg->next = alloc(0, sizeof(*arg));
            else
                arg->next = 0;
        }
    }
    
    // Call the constructor
    pro_ref ref;
    pro_constructor_call(s, constructor, arg_list, &ref);
    pro_release(s, constructor);
    
    // Release the argument list
    for (pro_ref_list arg = arg_list; arg; )
    {
        pro_release(s, arg->value);
        pro_ref_list old = arg;
        arg = arg->next;
        alloc(old, 0);
    }
    
    return ref;
}

static void constructor_expr_print(pro_state_ref s,
    const pro_expr* t, const char* end)
{
    assert(pro_expr_get_type(t) == PRO_CONSTRUCTOR_EXPR_TYPE);
    
    printf("<constructor %s arguments:", t->value.constructor.identifier);
    pro_print_expr_list(s, t->value.constructor.arguments, "");
    printf(">%s", end);
}


static void constructor_expr_release(pro_expr* t)
{
    free(t->value.constructor.identifier);
    pro_release_expr_list(t->value.constructor.arguments);
    free(t);
}

const pro_expr_type_info pro_constructor_expr_type_info = {
    .eval = constructor_expr_eval,
    .print = constructor_expr_print,
    .release = constructor_expr_release
};


PRO_INTERNAL pro_expr* pro_constructor_expr_create(
    char* identifier, pro_expr_list* arguments)
{
    pro_expr* t = pro_expr_create(PRO_CONSTRUCTOR_EXPR_TYPE);
    t->value.constructor.identifier = identifier;
    t->value.constructor.arguments = arguments;
    return t;
}
