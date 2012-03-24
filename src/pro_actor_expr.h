#ifndef prosopon_pro_actor_expr_h
#define prosopon_pro_actor_expr_h

#include "prosopon.h"

#include "pro_expr.h"
#include "pro_expr_type.h"


extern const pro_expr_type_info pro_actor_expr_type_info;


PRO_INTERNAL pro_expr* pro_actor_expr_create(pro_expr* behavior);

#endif
