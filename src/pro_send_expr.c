#include "pro_send_expr.h"

#include <stdio.h>
#include <assert.h>


static void send_expr_eval(pro_state* s, pro_expr* t)
{
    assert(pro_expr_get_type(t) == PRO_SEND_EXPR_TYPE);
}

static void send_expr_print(pro_state* s, pro_expr* t)
{
    assert(pro_expr_get_type(t) == PRO_SEND_EXPR_TYPE);
    
    pro_expr* identifier = t->value.binary.left;
    pro_expr* msg = t->value.binary.right;
    
    printf("<send identifier:");
    pro_print_expr(s, identifier);
    printf(" message:");
    pro_print_expr(s, msg);
    printf(">\n");
}

const pro_expr_type_info pro_send_expr_type_info = {
    .eval = send_expr_eval,
    .print = send_expr_print
};


PRO_INTERNAL pro_expr* pro_send_expr_create(pro_expr* identifier, pro_expr* msg)
{
    pro_type type = pro_expr_get_type(identifier);
    assert(type == PRO_IDENTIFIER_EXPR_TYPE || type == PRO_CONSTRUCTOR_EXPR_TYPE);
    assert(pro_expr_get_type(msg) == PRO_MESSAGE_EXPR_TYPE);

    pro_expr* t = pro_expr_create(PRO_SEND_EXPR_TYPE);
    t->value.binary.left = identifier;
    t->value.binary.right = msg;
    return t;
}
