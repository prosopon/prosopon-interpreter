#include "pro_expr.h"

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>


PRO_INTERNAL pro_ref pro_expr_create(pro_state_ref s, pro_expr_type type, pro_expr** data)
{
    pro_ref ref;
    pro_ud_create(s, sizeof(pro_expr), PRO_DEFAULT_UD_DECONSTRUCTOR, &ref);
    
    pro_ud_write(s, ref, (void**)data);
    (*data)->type = type;
    return ref;
}


PRO_INTERNAL pro_ref pro_eval_expr(pro_state_ref s, pro_ref ref)
{
    pro_expr* t;
    pro_ud_write(s, ref, (void**)&t);
    
    const pro_expr_type type = pro_expr_get_type(t);
    return pro_types[type]->eval(s, t);
}


PRO_INTERNAL void pro_print_expr(pro_state_ref s, const pro_expr* t, const char* end)
{
    assert(t);
    const pro_expr_type type = pro_expr_get_type(t);
    pro_types[type]->print(s, t, end);
}


PRO_INTERNAL void pro_release_expr(pro_state_ref s, pro_ref ref)
{
    pro_expr* t;
    pro_ud_write(s, ref, (void**)&t);
    
    const pro_expr_type type = pro_expr_get_type(t);
    //pro_types[type]->release(s, t);
}


PRO_INTERNAL pro_expr_type pro_expr_get_type(const pro_expr* t)
{
    assert(t);
    return t->type;
}
