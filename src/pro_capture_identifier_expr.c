#include "pro_capture_identifier_expr.h"

#include "pro_expr.h"

#include <prosopon/prosopon.h>
#include <prosopon/prosopon_macros.h>

#include <assert.h>
#include <stdio.h>
#include <string.h>


static pro_matching capture_identifier_match(pro_state_ref s,
    pro_ref t, pro_ref tData,
    pro_ref o, pro_ref oData)
{
    char* identifier;
    pro_ud_read(s, tData, (const void**)&identifier);
    pro_bind(s, o, identifier);
    return PRO_MATCH_SUCCEED;
}

const pro_actor_type_info pro_capture_identifier_type_info = {
    .match = capture_identifier_match,
    .to_string = 0
};

pro_actor_type pro_capture_identifier_type =
    "interpreter.capture_identifier_type";


static pro_ref identifier_expr_eval(pro_state_ref s, pro_ref ref, pro_expr* t)
{    
        
    pro_ref actor;
    pro_ref ud = pro_string_ud_create(s, t->value.identifier);
    pro_actor_create(s, pro_capture_identifier_type, 0, ud, &actor);
    pro_release(s, ud);
    
    return actor;
}

static void identifier_expr_print(pro_state_ref s,
    const pro_expr* t, const char* end)
{
    assert(pro_expr_get_type(t) == PRO_CAPTURE_IDENTIFIER_EXPR_TYPE);
    char* value = t->value.identifier;
    
    printf("<capture_identifier %s>\n", value);
}

static void identifier_expr_release(pro_state_ref s, void* data)
{
    pro_expr* t = data;
    pro_alloc* alloc;
    pro_get_alloc(s, &alloc);
    alloc(t->value.identifier, 0);
    PRO_DEFAULT_UD_DECONSTRUCTOR(s, data);
}


#pragma mark -
#pragma mark Internal

const pro_expr_type_info pro_capture_identifier_expr_type_info = {
    .eval = identifier_expr_eval,
    .print = identifier_expr_print,
};


PRO_INTERNAL
pro_ref pro_capture_identifier_expr_create(pro_state_ref s, 
    char* value)
{
    pro_expr* t;
    pro_ref ref = pro_expr_create(s, PRO_CAPTURE_IDENTIFIER_EXPR_TYPE,
        identifier_expr_release, &t);
    t->value.identifier = value;
    return ref;
}
