#include "test-expressions.h"

#include "prosopon.h"

#include "pro_expr.h"


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

static void test_create(void)
{
    pro_expr* t = pro_expr_create(PRO_LET_EXPR_TYPE);
    CU_ASSERT(0 != t);
    CU_ASSERT(PRO_LET_EXPR_TYPE == t->type);
}

static void test_get_type(void)
{
    pro_expr* t = pro_expr_create(PRO_LET_EXPR_TYPE);
    CU_ASSERT(pro_expr_get_type(t) == t->type);
    CU_ASSERT(PRO_LET_EXPR_TYPE == pro_expr_get_type(t));
}


static CU_TestInfo tests[] = {
    {"create", test_create},
    {"get_type", test_get_type},
    CU_TEST_INFO_NULL,
};


CU_SuiteInfo pro_expr_test_suite = {
    "expressions",
    init,
    cleanup,
    tests
};
