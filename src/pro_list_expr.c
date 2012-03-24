#include "pro_list_expr.h"

#include "pro_expr_list.h"

#include <assert.h>
#include <stdio.h>


static void list_expr_eval(pro_state* s, pro_expr* t)
{
    assert(pro_expr_get_type(t) == PRO_LIST_EXPR_TYPE);
    
    pro_expr_list* list = t->value.list;
    while (list) 
    {
        pro_expr* value = list->value;
        if (value)
            pro_eval_expr(s, value);
        list = list->next;
    }
}

static void list_expr_print(pro_state* s, pro_expr* t, const char* end)
{
    assert(pro_expr_get_type(t) == PRO_LIST_EXPR_TYPE);
    
    printf("<expr_list values:");
    pro_print_expr_list(s, t->value.list, "");
    printf(">%s", end);
}

const pro_expr_type_info pro_list_expr_type_info = {
    .eval = list_expr_eval,
    .print = list_expr_print
};


PRO_INTERNAL pro_expr* pro_list_expr_create(pro_expr_list* list)
{
    pro_expr* t = pro_expr_create(PRO_LIST_EXPR_TYPE);
    t->value.list = list;
    return t;
}


PRO_INTERNAL pro_expr* pro_list_expr_join(
    pro_expr* list1, pro_expr* list2)
{
    assert(list1); // o1 must be an expression but o2 may be null
    assert(pro_expr_get_type(list1) == PRO_LIST_EXPR_TYPE);
    assert(!list2 || pro_expr_get_type(list2) == PRO_LIST_EXPR_TYPE);
    
    pro_expr_list_join(list1->value.list, list2->value.list);
    return list1;
}