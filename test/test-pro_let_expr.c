#include "test-expressions.h"

#include "prosopon.h"

#include "pro_let_expr.h"
#include "pro_identifier_expr.h"
#include "pro_actor_expr.h"


static pro_state* state;


static int init(void)
{
    state = pro_state_create();
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

static void test_create_identifier_actor(void)
{
    pro_expr* identifier = pro_identifier_expr_create("id");
    pro_expr* actor = pro_actor_expr_create(0);
    pro_expr* t = pro_let_expr_create(identifier, actor);
    CU_ASSERT(t != 0);
    CU_ASSERT(pro_expr_get_type(t) == PRO_LET_EXPR_TYPE);
    
    pro_expr* left = t->value.binary.left;
    CU_ASSERT(left != 0);
    CU_ASSERT(pro_expr_get_type(left) == PRO_IDENTIFIER_EXPR_TYPE);
    
    pro_expr* right = t->value.binary.right;
    CU_ASSERT(right != 0);
    CU_ASSERT(pro_expr_get_type(right) == PRO_ACTOR_EXPR_TYPE);
}

static CU_TestInfo tests[] = {
    {"create_identifier_actor", test_create_identifier_actor},
    CU_TEST_INFO_NULL,
};


CU_SuiteInfo pro_let_expr_test_suite = {
    "pro_let_expr",
    init,
    cleanup,
    tests
};