#include "pro_capture_identifier_expr.h"

#include "prosopon_actor_type.h"

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

pro_actor_type_info capture_identifier_type = {
    .match = capture_identifier_match
};

static pro_ref identifier_expr_eval(pro_state_ref s, pro_expr* t)
{    
    pro_register_actor_type(s, "interpreter.capture_identifier_type", &capture_identifier_type);
    
    const char* identifier = t->value.identifier;
    
    pro_ref ud;
    size_t size = identifier ? sizeof(*identifier) * (strlen(identifier) + 1) : 0;
    pro_ud_create(s, size, PRO_DEFAULT_UD_DECONSTRUCTOR, &ud);
    
    if (identifier)
    {
        void* ud_ptr;
        pro_ud_write(s, ud, &ud_ptr);
        strcpy(ud_ptr, identifier);
    }
        
    pro_ref actor;
    pro_actor_create(s, "interpreter.capture_identifier_type", 0, ud, &actor);
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

static void string_expr_release(pro_state_ref s, void* data)
{
    pro_expr* t = data;
    pro_alloc* alloc;
    pro_get_alloc(s, &alloc);
    alloc(t->value.string, 0);
    alloc(t, 0);
}


#pragma mark -
#pragma mark Internal

const pro_expr_type_info pro_capture_identifier_expr_type_info = {
    .eval = identifier_expr_eval,
    .print = identifier_expr_print,
    .release = string_expr_release
};


PRO_INTERNAL pro_ref pro_capture_identifier_expr_create(pro_state_ref s, 
    char* value)
{
    pro_expr* t;
    pro_ref ref = pro_expr_create(s, PRO_CAPTURE_IDENTIFIER_EXPR_TYPE, &t);
    t->value.identifier = value;
    return ref;
}
