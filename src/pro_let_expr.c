#include "pro_let_expr.h"

#include "prosopon.h"

#include <assert.h>
#include <stdio.h>


static void let_expr_eval(pro_state* s, pro_expr* t)
{
    assert(pro_expr_get_type(t) == PRO_LET_EXPR_TYPE);
    
    pro_expr* left = t->value.binary.left;
    pro_expr* right = t->value.binary.right;
    
    switch (pro_expr_get_type(left))
    {
    case PRO_IDENTIFIER_EXPR_TYPE:
        pro_eval_expr(s, right);
        pro_bind(s, right->data.lookup, left->value.identifier);
        break;
    case PRO_CONSTRUCTOR_EXPR_TYPE:
        
        break;
    default:
        assert(0);
        break;
    }
}

static void let_expr_print(pro_state* s, pro_expr* t, const char* end)
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

const pro_expr_type_info pro_let_expr_type_info = {
    .eval = let_expr_eval,
    .print = let_expr_print
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
            value_type == PRO_NUMBER_EXPR_TYPE);
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
