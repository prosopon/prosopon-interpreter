#ifndef prosopon_pro_actor_expr_h
#define prosopon_pro_actor_expr_h

#include "prosopon.h"

#include "pro_expr.h"
#include "pro_expr_type.h"


extern const pro_expr_type_info pro_actor_expr_type_info;


PRO_INTERNAL pro_ref pro_actor_expr_create(pro_state_ref s,
    pro_ref behavior);

/**
 * Gets the behavior for a given actor expr
 *
 * @return
 */
PRO_INTERNAL pro_behavior* pro_actor_expr_get_behavior(pro_state_ref,
    pro_expr* t, pro_ref* ud);

#endif
