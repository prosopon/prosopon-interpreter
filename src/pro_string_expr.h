#ifndef prosopon_pro_string_expr_h
#define prosopon_pro_string_expr_h

#include <prosopon/prosopon.h>

#include "pro_expr_type.h"


extern const pro_expr_type_info pro_string_expr_type_info;


PRO_INTERNAL pro_ref pro_string_expr_create(pro_state_ref s, char*);

#endif
