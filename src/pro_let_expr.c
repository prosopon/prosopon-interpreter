#include "pro_let_expr.h"

#include "prosopon.h"

#include <assert.h>
#include <stdio.h>

#include "pro_actor_expr.h"


typedef struct {
    pro_ref constructor_expr;
    pro_ref actor_expr;
} constructor_data;


static void constructor_data_deconstructor(pro_state_ref s, void* data)
{
    constructor_data* t = data;
    
    pro_release(s, t->actor_expr);
    pro_release(s, t->constructor_expr);
    PRO_DEFAULT_UD_DECONSTRUCTOR(s, data);
}

static void bind_arguments(pro_state_ref s, pro_expr_list* id_list, pro_ref values)
{
    pro_expr_list* list = id_list;
    
    unsigned int len;
    pro_list_length(s, values, &len);
    for (unsigned int i = 0; i < len; ++i)
    {
        pro_ref value = list->value;
        pro_ref lookup;
        pro_list_get(s, values, i, &lookup);
        if (value)
        {
            pro_expr* val_expr;
            pro_ud_write(s, value, (void**)&val_expr);
            assert(pro_expr_get_type(val_expr) == PRO_IDENTIFIER_EXPR_TYPE);
            pro_bind(s, lookup, val_expr->value.identifier);
        }
        pro_release(s, lookup);
        list = list->next;
    }
}


static pro_ref contructor(pro_state_ref s, pro_ref arguments, pro_ref d)
{
    pro_ref actor = 0;
    const constructor_data* data;
    pro_ud_read(s, d, (const void**)&data);
    
    pro_ref constructor_ref = data->constructor_expr;
    pro_ref actor_ref = data->actor_expr;
    
    // get the constructor expression
    const pro_expr* constructor_expr;
    pro_ud_read(s, constructor_ref, (const void**)&constructor_expr);
    
    // bind all arguments in the environment
    bind_arguments(s, constructor_expr->value.constructor.arguments, arguments);
    
    // get the actor expression
    const pro_expr* actor_expr;
    pro_ud_read(s, actor_ref, (const void**)&actor_expr);
    
    // create a new actor in the new environment.
    pro_ref ud;
    pro_behavior* behavior = pro_actor_expr_get_behavior(s, actor_expr, &ud);
    pro_actor_create(s, PRO_DEFAULT_ACTOR_TYPE, behavior, ud, &actor);
    pro_release(s, ud);
    
    return actor;
}


static pro_ref let_expr_eval(pro_state_ref s, pro_ref ref, pro_expr* t)
{
    assert(pro_expr_get_type(t) == PRO_LET_EXPR_TYPE);
    
    pro_ref left_ref = t->value.binary.left;
    pro_ref right_ref = t->value.binary.right;
    
    const pro_expr* left;
    pro_ud_read(s, left_ref, (const void**)&left);
    
    switch (pro_expr_get_type(left))
    {
    case PRO_IDENTIFIER_EXPR_TYPE:
    {
        pro_ref right_eval = pro_eval_expr(s, right_ref);
        pro_bind(s, right_eval, left->value.identifier);
        pro_release(s, right_eval);
    }   break;
    case PRO_CONSTRUCTOR_EXPR_TYPE:
    {
        constructor_data* cData;
        pro_ref ud;
        pro_ud_create(s, sizeof(*cData), constructor_data_deconstructor, &ud);
        pro_ud_write(s, ud, (void**)&cData);
        
        pro_retain(s, right_ref);
        pro_retain(s, left_ref);
        cData->actor_expr = right_ref;
        cData->constructor_expr = left_ref;
        
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
    pro_ref value = t->value.binary.right;
    
    printf("<let identifier:");
    pro_print_expr(s, identifier, " ");
    printf("value:");
    pro_print_expr(s, value, "");
    printf(">%s", end);
}


static void let_expr_release(pro_state_ref s, void* data)
{
    pro_expr* t = data;
    pro_release(s, t->value.binary.left);
    pro_release(s, t->value.binary.right);
    PRO_DEFAULT_UD_DECONSTRUCTOR(s, data);
}


#pragma mark -
#pragma mark Internal

const pro_expr_type_info pro_let_expr_type_info = {
    .eval = let_expr_eval,
    .print = let_expr_print,
};


PRO_INTERNAL pro_ref pro_let_expr_create(pro_state_ref s,
    pro_ref identifier, pro_ref value)
{
    pro_expr* t;
    pro_ref ref = pro_expr_create(s, PRO_LET_EXPR_TYPE, let_expr_release, &t);
    t->value.binary.left = identifier;
    t->value.binary.right = value;
    return ref;
}
