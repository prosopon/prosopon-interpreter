#include "pro_expr_list.h"


#include <assert.h>
#include <stdio.h>
#include <stdlib.h>



PRO_INTERNAL void pro_print_expr_list(pro_state* s, pro_expr_list* t, const char* end)
{    
    pro_expr_list* list = t;
    while (list) 
    {
        pro_expr* value = list->value;
        if (value)
            pro_print_expr(s, value, list->next ? ", " : "");
        list = list->next;
    }
    
    printf("%s", end);
}


PRO_INTERNAL pro_expr_list* pro_expr_list_create(
    pro_expr* value, pro_expr_list* next)
{
    pro_expr_list* t = malloc(sizeof(*t));
    t->value = value;
    t->next = next;
    return t;
}

/**
 * @returns The last element of an expression list or null if none.
 */
static pro_expr_list* pro_expr_list_tail(pro_expr_list* t)
{
    if (!t)
        return 0;
    else
    {
        pro_expr_list* tail = t;
        for (pro_expr_list* next; (next = tail->next);)
             tail = next;
        return tail;
    }
}

PRO_INTERNAL pro_expr_list* pro_expr_list_join(pro_expr_list* o1, pro_expr_list* o2)
{
    assert(o1); // o1 must be an expression but o2 may be null
    pro_expr_list* tail = pro_expr_list_tail(o1);
    tail->next = o2;
    return o1;
}

