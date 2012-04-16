#ifndef prosopon_pro_expr_list_h
#define prosopon_pro_expr_list_h

#include "prosopon.h"
#include "prosopon_config.h"

#include "pro_expr.h"
#include "pro_expr_type.h"


/**
 * Creates a new expression list.
 *
 * Wraps an existing expression in a list expression.
 *
 * @returns The new list expression.
 */
PRO_INTERNAL pro_expr_list* pro_expr_list_create(pro_state_ref s,
    pro_expr* value, pro_expr_list* next);


/**
 * Appends expression list 2 to expression list 1.
 *
 * @returns The combined expression list.
 */
PRO_INTERNAL pro_expr_list* pro_expr_list_join(
    pro_expr_list* list1, pro_expr_list* list2);


/**
 * Prints out an expression list.
 */
PRO_INTERNAL void pro_print_expr_list(pro_state_ref, pro_expr_list*, const char* end);


/**
 * Releases an expression list for collection.
 */
PRO_INTERNAL void pro_release_expr_list(pro_state_ref, pro_expr_list*);

#endif
