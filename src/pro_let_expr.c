#include "pro_let_expr.h"

#include "prosopon.h"

#include <assert.h>
#include <stdio.h>


static void let_expr_eval(pro_state* s, pro_expr* t)
{
    assert(pro_expr_get_type(s, t) == PRO_LET_EXPR_TYPE);
    
    pro_expr* left = t->value.binary.left;
    pro_expr* right = t->value.binary.right;
    
    switch (pro_expr_get_type(s, left))
    {
    case PRO_IDENTIFIER_EXPR_TYPE:
        pro_eval_expr(s, right);
        pro_lookup_bind(s, right->data.lookup, left->value.identifier);
        break;
    case PRO_CONSTRUCTOR_EXPR_TYPE:
        
        break;
    default:
        assert(0);
        break;
    }
}

static void let_expr_print(pro_state* s, pro_expr* t)
{
    assert(pro_expr_get_type(s, t) == PRO_LET_EXPR_TYPE);
    pro_expr* identifier =  t->value.binary.left;
    pro_expr* value = t->value.binary.right;
    
    printf("<let identifier:");
    pro_print_expr(s, identifier);
    printf(" value:");
    pro_print_expr(s, value);
    printf(">\n");
}

const pro_expr_type_info pro_let_expr_type_info = {
    .eval = let_expr_eval,
    .print = let_expr_print
};


PRO_INTERNAL pro_expr* pro_let_expr_create(pro_state* s, pro_expr* identifier, pro_expr* value)
{
    pro_expr* t = pro_expr_create(s, PRO_LET_EXPR_TYPE);
    t->value.binary.left = identifier;
    t->value.binary.right = value;
    return t;
}
