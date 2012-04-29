#include "test-expressions.h"

#include <prosopon/prosopon.h>

#include "pro_become_expr.h"
#include "pro_identifier_expr.h"
#include "pro_actor_expr.h"
#include "pro_constructor_expr.h"
#include "pro_message_expr.h"
#include "pro_number_expr.h"


static pro_state_ref state;


static int init(void)
{
    pro_state_create(test_alloc, &state);
    return state == 0;
}

static int cleanup(void)
{
    pro_state_release(state);
    state = 0;
    return 0;
}


#pragma mark -
#pragma mark Tests


static void test_create_actor(void)
{
    pro_expr* id = pro_identifier_expr_create("id");
    pro_expr* actor = pro_actor_expr_create(0);
    pro_expr* t = pro_become_expr_create(id, actor);
    CU_ASSERT(t != 0);
    CU_ASSERT(pro_expr_get_type(t) == PRO_BECOME_EXPR_TYPE);
    
    pro_expr* left = t->value.binary.left;
    CU_ASSERT(left != 0);
    CU_ASSERT(pro_expr_get_type(left) == PRO_IDENTIFIER_EXPR_TYPE);
    
    pro_expr* right = t->value.binary.right;
    CU_ASSERT(right != 0);
    CU_ASSERT(pro_expr_get_type(right) == PRO_ACTOR_EXPR_TYPE);
}


static void test_create_constructor(void)
{
    pro_expr* id = pro_identifier_expr_create("id");
    pro_expr* constructor = pro_constructor_expr_create("id", 0);
    pro_expr* t = pro_become_expr_create(id, constructor);
    CU_ASSERT(t != 0);
    CU_ASSERT(pro_expr_get_type(t) == PRO_BECOME_EXPR_TYPE);
    
    pro_expr* left = t->value.binary.left;
    CU_ASSERT(left != 0);
    CU_ASSERT(pro_expr_get_type(left) == PRO_IDENTIFIER_EXPR_TYPE);
    
    pro_expr* right = t->value.binary.right;
    CU_ASSERT(right != 0);
    CU_ASSERT(pro_expr_get_type(right) == PRO_CONSTRUCTOR_EXPR_TYPE);
}


static CU_TestInfo tests[] = {
    {"create_actor", test_create_actor},
    {"create_constructor", test_create_constructor},
    CU_TEST_INFO_NULL,
};


CU_SuiteInfo pro_become_expr_test_suite = {
    "pro_become_expr",
    init,
    cleanup,
    tests
};
