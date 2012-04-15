#include "pro_let_expr.h"

#include "prosopon.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "pro_actor_expr.h"


typedef struct {
    pro_env_ref env;
    pro_expr* constructor_expr;
    pro_expr* actor_expr;
} constructor_data;


static void constructor_data_deconstructor(pro_state_ref s, void* data)
{
    pro_alloc* alloc;
    pro_get_alloc(s, &alloc);
    
    alloc(data, 0);
}

static void bind_arguments(pro_state_ref s, pro_expr_list* id_list, pro_ref_list values)
{
    pro_expr_list* list = id_list;
    pro_ref_list lookup_list = values;
    while (list)
    {
        pro_expr* value = list->value;
        pro_ref lookup = lookup_list->value;
        if (value)
        {
            assert(pro_expr_get_type(value) == PRO_IDENTIFIER_EXPR_TYPE);
            pro_bind(s, lookup, value->value.identifier);
        }
        list = list->next;
        lookup_list = lookup_list->next;
    }
}


static pro_ref contructor(pro_state_ref s, pro_ref_list arguments, pro_ref d)
{
    pro_ref actor = 0;
    const constructor_data* data;
    pro_ud_read(s, d, (const void**)&data);
    
    pro_expr* constructor_expr = data->constructor_expr;
    pro_expr* actor_expr = data->actor_expr;

    // Create a new environment and make it current.
    {
        pro_env_ref env;
        pro_env_create(s, data->env, &env);
        pro_push_env(s, env);
        pro_env_release(s, env);
    }
    
    // bind all arguments in the new environment
    bind_arguments(s, constructor_expr->value.constructor.arguments, arguments);
    
    // Create a new actor in the new environment.
    pro_ref ud;
    pro_behavior* behavior = pro_actor_expr_get_behavior(s, actor_expr, &ud);
    pro_actor_create(s, PRO_DEFAULT_ACTOR_TYPE, behavior, ud, &actor);
    pro_release(s, ud);
    
    // Restore the old environment.
    pro_pop_env(s);
    
    return actor;
}


static pro_ref let_expr_eval(pro_state_ref s, pro_expr* t)
{
    assert(pro_expr_get_type(t) == PRO_LET_EXPR_TYPE);
    
    pro_expr* left = t->value.binary.left;
    pro_expr* right = t->value.binary.right;
    
    switch (pro_expr_get_type(left))
    {
    case PRO_IDENTIFIER_EXPR_TYPE:
    {
        pro_ref right_ref = pro_eval_expr(s, right);
        pro_bind(s, right_ref, left->value.identifier);
        pro_release(s, right_ref);
    }   break;
    case PRO_CONSTRUCTOR_EXPR_TYPE:
    {
        constructor_data* cData = 0;
        pro_ref ud;
        pro_ud_create(s, sizeof(*cData), constructor_data_deconstructor, &ud);
        pro_ud_write(s, ud, (void**)&cData);
        
        pro_env_ref env;
        pro_get_env(s, &env);
        cData->actor_expr = right;
        cData->constructor_expr = left;
        cData->env = env;
        //pro_env_release(s, env);
        
        pro_ref lookup;
        pro_constructor_create(s, contructor, ud, &lookup);
        pro_bind(s, lookup, left->value.identifier);
        pro_release(s, ud);
        pro_release(s, lookup);
    }   break;
    default:
        assert(0);
        break;
    }
    return PRO_EMPTY_REF;
}


static void let_expr_print(pro_state_ref s, const pro_expr* t, const char* end)
{
    assert(pro_expr_get_type(t) == PRO_LET_EXPR_TYPE);
    pro_expr* identifier =  t->value.binary.left;
    pro_expr* value = t->value.binary.right;
    
    printf("<let identifier:");
    pro_print_expr(s, identifier, " ");
    printf("value:");
    pro_print_expr(s, value, "");
    printf(">%s", end);
}


static void let_expr_release(pro_expr* t)
{
    pro_release_expr(t->value.binary.left);
    pro_release_expr(t->value.binary.right);
    free(t);
}


#pragma mark -
#pragma mark Internal

const pro_expr_type_info pro_let_expr_type_info = {
    .eval = let_expr_eval,
    .print = let_expr_print,
    .release = let_expr_release
};


PRO_INTERNAL pro_expr* pro_let_expr_create(pro_expr* identifier, pro_expr* value)
{
    pro_type identifier_type = pro_expr_get_type(identifier);
    pro_type value_type = pro_expr_get_type(value);

    switch (identifier_type)
    {
    case PRO_IDENTIFIER_EXPR_TYPE:
        assert(
            value_type == PRO_ACTOR_EXPR_TYPE ||
            value_type == PRO_STRING_EXPR_TYPE ||
            value_type == PRO_NUMBER_EXPR_TYPE ||
            value_type == PRO_CONSTRUCTOR_EXPR_TYPE);
        break;
    case PRO_CONSTRUCTOR_EXPR_TYPE:
        assert(value_type == PRO_ACTOR_EXPR_TYPE);
        break;
    default:
        assert(0);
        break;
    }
    
    pro_expr* t = pro_expr_create(PRO_LET_EXPR_TYPE);
    t->value.binary.left = identifier;
    t->value.binary.right = value;
    return t;
}
