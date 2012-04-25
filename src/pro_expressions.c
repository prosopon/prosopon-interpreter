#include "pro_expressions.h"


PRO_INTERNAL void pro_register_expr_types(pro_state_ref s)
{
    pro_register_actor_type(s, pro_capture_identifier_type,
        &pro_capture_identifier_type_info);
}