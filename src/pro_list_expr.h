#ifndef prosopon_pro_list_expr_h
#define prosopon_pro_list_expr_h

#include <prosopon/prosopon.h>

#include "pro_expr.h"
#include "pro_expr_type.h"


extern const pro_expr_type_info pro_list_expr_type_info;


PRO_INTERNAL
pro_ref pro_list_expr_create(pro_state_ref s,
    pro_expr_list* list);


    
PRO_INTERNAL
pro_ref pro_list_expr_append(pro_state_ref s,
     pro_ref expr_ref, pro_ref val_ref);

#endif
