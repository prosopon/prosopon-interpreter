#include "pro_message_expr.h"

#include <stdio.h>
#include <assert.h>

#include "pro_expr_list.h"


static void message_expr_eval(pro_state* s, pro_expr* t)
{
    assert(pro_expr_get_type(t) == PRO_MESSAGE_EXPR_TYPE);
}

static void message_expr_print(pro_state* s, pro_expr* t, const char* end)
{
    assert(pro_expr_get_type(t) == PRO_MESSAGE_EXPR_TYPE);
    printf("<message ");
    pro_print_expr_list(s, t->value.list, "");
    printf(">%s", end);
}


const pro_expr_type_info pro_message_expr_type_info = {
    .eval = message_expr_eval,
    .print = message_expr_print
};


PRO_INTERNAL pro_expr* pro_message_expr_create(pro_expr_list* list)
{
    pro_expr* t = pro_expr_create(PRO_MESSAGE_EXPR_TYPE);
    t->value.list = list;
    return t;
}
