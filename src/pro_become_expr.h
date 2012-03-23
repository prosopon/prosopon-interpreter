#ifndef prosopon_pro_become_expr_h
#define prosopon_pro_become_expr_h

#include "prosopon.h"

#include "pro_expr.h"
#include "pro_expr_type.h"


extern const pro_expr_type_info pro_become_expr_type_info;


PRO_INTERNAL pro_expr* pro_become_expr_create(pro_state*, pro_expr* identifier, pro_expr* value);

PRO_INTERNAL pro_expr* pro_become_constructor_expr_create(pro_state*, pro_expr* constructor);

#endif
