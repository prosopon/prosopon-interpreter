#include "pro_expr_list.h"


#include <assert.h>
#include <stdio.h>


#pragma mark Private

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


#pragma mark -
#pragma mark Internal

PRO_INTERNAL void pro_print_expr_list(pro_state_ref s, pro_expr_list* t, const char* end)
{    
    pro_expr_list* list = t;
    while (list) 
    {
        pro_ref value = list->value;
        if (value)
            pro_print_expr(s, value, list->next ? ", " : "");
        list = list->next;
    }
    
    printf("%s", end);
}


PRO_INTERNAL pro_expr_list* pro_expr_list_create(pro_state_ref s, 
    pro_ref expr, pro_expr_list* next)
{
    pro_alloc* alloc;
    pro_get_alloc(s, &alloc);
    pro_expr_list* t = alloc(0, sizeof(*t));
    if (!t) return 0;
    
    t->value = expr;
    t->next = next;
    return t;
}



PRO_INTERNAL pro_expr_list* pro_expr_list_join(pro_expr_list* o1, pro_expr_list* o2)
{
    assert(o1); // o1 must be an expression but o2 may be null
    pro_expr_list* tail = pro_expr_list_tail(o1);
    tail->next = o2;
    return o1;
}


PRO_INTERNAL void pro_release_expr_list(pro_state_ref s, pro_expr_list* t)
{
    pro_alloc* alloc;
    pro_get_alloc(s, &alloc);
    
    pro_expr_list* list = t;
    while (list) 
    {
        pro_expr_list* old = list;
        pro_ref value = list->value;
        
        if (value)
            pro_release(s, value);
        list = list->next;
        
        alloc(old, 0);
    }
}


