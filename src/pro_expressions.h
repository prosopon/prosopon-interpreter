#ifndef prosopon_pro_expressions_h
#define prosopon_pro_expressions_h

#include <prosopon/prosopon.h>


/**
 * Includes for all expression headers.
 */
#include "pro_list_expr.h"
#include "pro_let_expr.h"
#include "pro_string_expr.h"
#include "pro_expr_list.h"
#include "pro_number_expr.h"
#include "pro_capture_identifier_expr.h"
#include "pro_identifier_expr.h"
#include "pro_send_expr.h"
#include "pro_become_expr.h"
#include "pro_case_expr.h"
#include "pro_message_expr.h"
#include "pro_actor_expr.h"
#include "pro_constructor_expr.h"


PRO_INTERNAL
void pro_register_expr_types(pro_state_ref);

#endif
