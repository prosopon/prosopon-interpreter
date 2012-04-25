#ifndef prosopon_pro_send_expr_h
#define prosopon_pro_send_expr_h

#include <prosopon/prosopon.h>

#include "pro_expr.h"


extern const pro_expr_type_info pro_send_expr_type_info;


PRO_INTERNAL pro_ref pro_send_expr_create(pro_state_ref s,
    pro_ref identifier, pro_ref msg);


#endif
