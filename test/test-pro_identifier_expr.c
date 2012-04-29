#include "test-expressions.h"

#include <prosopon/prosopon.h>

#include "pro_expr.h"
#include "pro_identifier_expr.h"

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
    char* str = "abcd";
    pro_expr* t = pro_identifier_expr_create(str);
    CU_ASSERT(pro_expr_get_type(t) == PRO_IDENTIFIER_EXPR_TYPE);
    CU_ASSERT_STRING_EQUAL(str, t->value.identifier);
}


static CU_TestInfo tests[] = {
    {"create", test_create},
    CU_TEST_INFO_NULL,
};


CU_SuiteInfo pro_identifier_expr_test_suite = {
    "pro_identifier_expr",
    init,
    cleanup,
    tests
};
