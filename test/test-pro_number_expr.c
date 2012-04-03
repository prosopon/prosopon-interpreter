#include "test-expressions.h"

#include "prosopon.h"

#include "pro_expr.h"
#include "pro_number_expr.h"

static pro_state_ref state;


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

static void test_create(void)
{
    double value = 10.34;
    pro_expr* t = pro_number_expr_create(value);
    CU_ASSERT(pro_expr_get_type(t) == PRO_NUMBER_EXPR_TYPE);
    CU_ASSERT(value == t->value.number);
}


static CU_TestInfo tests[] = {
    {"create", test_create},
    CU_TEST_INFO_NULL,
};


CU_SuiteInfo pro_number_expr_test_suite = {
    "pro_number_expr",
    init,
    cleanup,
    tests
};
