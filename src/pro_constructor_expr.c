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

    pro_ref arg_list;
    pro_list_create(s, &arg_list);

    for (pro_expr_list* expr_arg = expr_arg_list; expr_arg; expr_arg = expr_arg->next)
    {
        pro_expr* value = expr_arg->value;
        if (value)
        {
            pro_ref new_list = PRO_EMPTY_REF;
            pro_ref lookup = pro_eval_expr(s, value);
            pro_list_append(s, arg_list, lookup, &new_list);
            pro_release(s, lookup);
            pro_release(s, arg_list);
            arg_list = new_list;
        }
    }
    
    // Call the constructor
    pro_ref ref;
    pro_constructor_call(s, constructor, arg_list, &ref);
    pro_release(s, constructor);
    
    pro_release(s, arg_list);
    
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
