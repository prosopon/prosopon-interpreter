#ifndef prosopon_pro_string_expr_h
#define prosopon_pro_string_expr_h

#include "prosopon.h"

#include "pro_expr.h"
#include "pro_expr_type.h"


extern const pro_expr_type_info pro_string_expr_type_info;


PRO_INTERNAL pro_expr* pro_string_expr_create(char*);

#endif
