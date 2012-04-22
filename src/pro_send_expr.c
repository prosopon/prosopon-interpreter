#include "pro_send_expr.h"

#include <stdio.h>
#include <assert.h>


#pragma mark Private

static pro_ref send_expr_eval(pro_state_ref s, pro_expr* t)
{
    assert(pro_expr_get_type(t) == PRO_SEND_EXPR_TYPE);
    
    pro_ref left = t->value.binary.left;
    pro_ref msg = t->value.binary.right;
    
    pro_ref target_val = pro_eval_expr(s, left);
    pro_ref msg_val = pro_eval_expr(s, msg);
    pro_send(s, target_val,  msg_val);
    pro_release(s, target_val);
    pro_release(s, msg_val);
    
    return PRO_EMPTY_REF;
}

static void send_expr_print(pro_state_ref s, const pro_expr* t, const char* end)
{
    assert(pro_expr_get_type(t) == PRO_SEND_EXPR_TYPE);
    
    pro_ref identifier = t->value.binary.left;
    pro_ref msg = t->value.binary.right;
    
    printf("<send identifier:");
    pro_print_expr(s, identifier, " ");
    printf("message:");
    pro_print_expr(s, msg, "");
    printf(">%s", end);
}

static void send_expr_release(pro_state_ref s, void* data)
{
    pro_expr* t = data;
    pro_alloc* alloc;
    pro_get_alloc(s, &alloc);
    pro_release(s, t->value.binary.left);
    pro_release(s, t->value.binary.right);
    alloc(t, 0);
}


#pragma mark -
#pragma mark Internal

const pro_expr_type_info pro_send_expr_type_info = {
    .eval = send_expr_eval,
    .print = send_expr_print,
    .release = send_expr_release
};


PRO_INTERNAL pro_ref pro_send_expr_create(pro_state_ref s,
    pro_ref identifier, pro_ref msg)
{
    pro_expr* t;
    pro_ref ref = pro_expr_create(s, PRO_SEND_EXPR_TYPE, &t);
    t->value.binary.left = identifier;
    t->value.binary.right = msg;
    return ref;
}
