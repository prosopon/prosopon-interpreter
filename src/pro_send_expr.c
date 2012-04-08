#include "pro_send_expr.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>


#pragma mark Private

static void send_expr_eval(pro_state_ref s, pro_expr* t)
{
    assert(pro_expr_get_type(t) == PRO_SEND_EXPR_TYPE);
    
    pro_expr* left = t->value.binary.left;
    pro_expr* msg = t->value.binary.right;
    
    pro_eval_expr(s, left);
    pro_eval_expr(s, msg);
    pro_send(s, left->data.lookup, msg->data.lookup);
}

static void send_expr_print(pro_state_ref s, const pro_expr* t, const char* end)
{
    assert(pro_expr_get_type(t) == PRO_SEND_EXPR_TYPE);
    
    pro_expr* identifier = t->value.binary.left;
    pro_expr* msg = t->value.binary.right;
    
    printf("<send identifier:");
    pro_print_expr(s, identifier, " ");
    printf("message:");
    pro_print_expr(s, msg, "");
    printf(">%s", end);
}

static void send_expr_release(pro_expr* t)
{
    pro_release_expr(t->value.binary.left);
    pro_release_expr(t->value.binary.right);
    free(t);
}


#pragma mark -
#pragma mark Internal

const pro_expr_type_info pro_send_expr_type_info = {
    .eval = send_expr_eval,
    .print = send_expr_print,
    .release = send_expr_release
};


PRO_INTERNAL pro_expr* pro_send_expr_create(pro_expr* identifier, pro_expr* msg)
{
    pro_type type = pro_expr_get_type(identifier);
    assert(
        type == PRO_IDENTIFIER_EXPR_TYPE ||
        type == PRO_CONSTRUCTOR_EXPR_TYPE ||
        type == PRO_STRING_EXPR_TYPE ||
        type == PRO_NUMBER_EXPR_TYPE ||
        type == PRO_ACTOR_EXPR_TYPE);
    pro_type msg_type = pro_expr_get_type(msg);
    assert(
        PRO_MESSAGE_EXPR_TYPE == msg_type ||
        PRO_IDENTIFIER_EXPR_TYPE == msg_type);

    pro_expr* t = pro_expr_create(PRO_SEND_EXPR_TYPE);
    t->value.binary.left = identifier;
    t->value.binary.right = msg;
    return t;
}
