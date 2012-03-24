#include "test-expressions.h"

#include "prosopon.h"


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



static CU_TestInfo tests[] = {
    CU_TEST_INFO_NULL,
};


CU_SuiteInfo pro_let_expr_test_suite = {
    "pro_let_expr",
    init,
    cleanup,
    tests
};
