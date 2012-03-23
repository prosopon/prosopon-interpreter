#include "pro_message_expr.h"

#include <stdio.h>
#include <assert.h>


static void message_expr_eval(pro_state* s, pro_expr* t)
{
    assert(pro_expr_get_type(s, t) == PRO_MESSAGE_EXPR_TYPE);
}

static void message_expr_print(pro_state* s, pro_expr* t)
{
    assert(pro_expr_get_type(s, t) == PRO_MESSAGE_EXPR_TYPE);
}


const pro_expr_type_info pro_message_expr_type_info = {
    .eval = message_expr_eval,
    .print = message_expr_print
};


PRO_INTERNAL pro_expr* pro_message_expr_create(pro_state* s, pro_expr_list* list)
{
    pro_expr* t = pro_expr_create(s, PRO_MESSAGE_EXPR_TYPE);
    t->value.list = list;
    return t;
}
