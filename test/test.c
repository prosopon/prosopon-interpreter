#include "CUnit/Basic.h"

#include "test-expressions.h"



int main(int argc, char** argv)
{
    CU_SuiteInfo suites[] = {
        pro_expr_test_suite,
        pro_identifier_expr_test_suite,
        pro_string_expr_test_suite,
        pro_number_expr_test_suite,
        pro_let_expr_test_suite,
        CU_SUITE_INFO_NULL
    };
    
    // initialize the CUnit test registry
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    // add suites to the registry
    if (CUE_SUCCESS != CU_register_suites(suites))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // run all tests
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}