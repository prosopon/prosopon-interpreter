#include "pro_expr_type.h"

#include "pro_expressions.h"


/**
 * Definition of type lookup table info.
 *
 * Order should match pro_expr_type order.
 */
const pro_expr_type_info* pro_types[PRO_MAX_EXPR_TYPE] = {
    &pro_list_expr_type_info,
    &pro_let_expr_type_info,
    &pro_send_expr_type_info,
    &pro_become_expr_type_info,
    &pro_case_expr_type_info,
    &pro_string_expr_type_info,
    &pro_number_expr_type_info,
    &pro_capture_identifier_expr_type_info,
    &pro_identifier_expr_type_info,
    &pro_message_expr_type_info,
    &pro_actor_expr_type_info,
    &pro_constructor_expr_type_info
};