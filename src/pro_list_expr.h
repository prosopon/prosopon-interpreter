#ifndef prosopon_pro_list_expr_h
#define prosopon_pro_list_expr_h

#include "prosopon.h"

#include "pro_expr.h"
#include "pro_expr_type.h"


extern const pro_expr_type_info pro_list_expr_type_info;


PRO_INTERNAL pro_expr* pro_list_expr_create(pro_state_ref s,
    pro_expr_list* list);

/**
 * Appends expression list 2 to expression list 1.
 *
 * @returns The combined expression list.
 */
PRO_INTERNAL pro_expr* pro_list_expr_join(
    pro_expr* list1, pro_expr* list2);
    
PRO_INTERNAL pro_expr* pro_list_expr_append(pro_state_ref s,
    pro_expr* expr, pro_expr* list);

#endif
