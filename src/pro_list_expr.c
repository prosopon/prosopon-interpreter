#include "pro_list_expr.h"

#include "pro_expr_list.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>


static pro_ref list_expr_eval(pro_state_ref s, pro_ref ref, pro_expr* t)
{
    assert(pro_expr_get_type(t) == PRO_LIST_EXPR_TYPE);
    
    for (pro_expr_list* list = t->value.list; list; list = list->next) 
    {
        pro_ref value = list->value;
        if (value)
            pro_release(s, pro_eval_expr(s, value));
    }
    return PRO_EMPTY_REF;
}

static void list_expr_print(pro_state_ref s, const pro_expr* t, const char* end)
{
    assert(pro_expr_get_type(t) == PRO_LIST_EXPR_TYPE);
    
    printf("<expr_list values:");
    pro_print_expr_list(s, t->value.list, "");
    printf(">%s", end);
}

static void list_expr_release(pro_state_ref s, void* data)
{
    pro_expr* t = data;
    pro_release_expr_list(s, t->value.list);
    
    PRO_DEFAULT_UD_DECONSTRUCTOR(s, data);
}

const pro_expr_type_info pro_list_expr_type_info = {
    .eval = list_expr_eval,
    .print = list_expr_print,
};


PRO_INTERNAL pro_ref pro_list_expr_create(pro_state_ref s,
    pro_expr_list* list)
{
    pro_expr* t;
    pro_ref ref = pro_expr_create(s, PRO_LIST_EXPR_TYPE, list_expr_release, &t);
    t->value.list = list;
    return ref;
}


PRO_INTERNAL pro_ref pro_list_expr_append(pro_state_ref s,
    pro_ref expr_ref, pro_ref val_ref)
{
    pro_expr* expr;
    pro_ud_write(s, expr_ref, (void**)&expr);

    pro_expr_list_join(expr->value.list, pro_expr_list_create(s, val_ref, 0));
    return expr_ref;
}


