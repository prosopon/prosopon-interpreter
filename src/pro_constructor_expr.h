#ifndef prosopon_pro_constructor_expr_h
#define prosopon_pro_constructor_expr_h

#include <prosopon/prosopon.h>

#include "pro_expr_type.h"


extern const pro_expr_type_info pro_constructor_expr_type_info;


PRO_INTERNAL pro_ref pro_constructor_expr_create(pro_state_ref s, 
    char* identifier, pro_ref arguments);

#endif
