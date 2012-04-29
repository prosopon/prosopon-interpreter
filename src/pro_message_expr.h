#ifndef prosopon_pro_message_expr_h
#define prosopon_pro_message_expr_h

#include <prosopon/prosopon.h>

#include "pro_expr_type.h"


extern const pro_expr_type_info pro_message_expr_type_info;


PRO_INTERNAL
pro_ref pro_message_expr_create(pro_state_ref s,
    pro_ref list);

#endif
