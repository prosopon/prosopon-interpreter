#ifndef prosopon_pro_let_expr_h
#define prosopon_pro_let_expr_h

#include <prosopon/prosopon.h>

#include "pro_expr_type.h"


extern const pro_expr_type_info pro_let_expr_type_info;


PRO_INTERNAL
pro_ref pro_let_expr_create(pro_state_ref s,
    pro_ref identifier, pro_ref value);

#endif
