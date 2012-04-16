#ifndef prosopon_pro_case_expr_h
#define prosopon_pro_case_expr_h

#include "prosopon.h"

#include "pro_expr.h"
#include "pro_expr_type.h"


extern const pro_expr_type_info pro_case_expr_type_info;



PRO_INTERNAL pro_expr* pro_case_expr_create(pro_state_ref s,
    pro_expr* pattern, pro_expr* body);


PRO_INTERNAL int pro_case_expr_match(pro_state_ref,
    pro_expr* t, pro_ref msg);

#endif
