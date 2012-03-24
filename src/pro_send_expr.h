#ifndef prosopon_pro_send_expr_h
#define prosopon_pro_send_expr_h

#include "prosopon.h"
#include "pro_expr.h"


extern const pro_expr_type_info pro_send_expr_type_info;


PRO_INTERNAL pro_expr* pro_send_expr_create(pro_expr* identifier, pro_expr* msg);


#endif
