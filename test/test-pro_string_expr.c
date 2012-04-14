#include "test-expressions.h"

#include "prosopon.h"

#include "pro_expr.h"
#include "pro_string_expr.h"

static pro_state_ref state;


static int init(void)
{
    pro_state_create(&state);
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
    char* str = "abcd \n";
    pro_expr* t = pro_string_expr_create(str);
    CU_ASSERT(pro_expr_get_type(t) == PRO_STRING_EXPR_TYPE);
    CU_ASSERT_STRING_EQUAL(str, t->value.identifier);
}


static CU_TestInfo tests[] = {
    {"create", test_create},
    CU_TEST_INFO_NULL,
};


CU_SuiteInfo pro_string_expr_test_suite = {
    "pro_string_expr",
    init,
    cleanup,
    tests
};
