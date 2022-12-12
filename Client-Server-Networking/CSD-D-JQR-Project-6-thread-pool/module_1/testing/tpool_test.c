#include "../include/tpool.h"
#include "../include/free.h"
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>

tpool_t * p_tpool = NULL;

void job1 ()
{
    srand(time(NULL));
    int sum = 0;
    int a = rand() % 100;
    int b = rand() % 100;
    sum = a + b;
    printf("\nThe sum of %d and %d is %d.\n", a, b , sum);
}

void job2 ()
{
    int prod = 0;
    int c = rand() % 100;
    int d = rand() % 100;
    prod = c * d;
    printf("\nThe product of %d and %d is %d.\n", c, d , prod);
}

void job3 ()
{
    int sum = 0;
    for (int i = 0; i < 10000; i++)
    {
        // printf("%d\n", i);
        sum += i;
    }
    printf("\nFinal Sum: %d\n", sum);

}


int init_suite1 ()
{
    return 0;
}

int clean_suite1 ()
{
    return 0;
}




void test_tpool_create()
{
    p_tpool = tpool_create(4);
    CU_ASSERT_FATAL(NULL != p_tpool);
    if (p_tpool)
    {
        CU_ASSERT(NULL != p_tpool->p_job_queue);
    }
}

void test_tpool_add_job()
{
    CU_ASSERT(true == tpool_add_job(p_tpool, job1, NULL, NULL));
    CU_ASSERT(true == tpool_add_job(p_tpool, job2, NULL, NULL));
    CU_ASSERT(true == tpool_add_job(p_tpool, job1, NULL, NULL));
    CU_ASSERT(true == tpool_add_job(p_tpool, job2, NULL, NULL));
    CU_ASSERT(true == tpool_add_job(p_tpool, job1, NULL, NULL));
    CU_ASSERT(true == tpool_add_job(p_tpool, job2, NULL, NULL));
    CU_ASSERT(true == tpool_add_job(p_tpool, job1, NULL, NULL));
    CU_ASSERT(true == tpool_add_job(p_tpool, job2, NULL, NULL));
    CU_ASSERT(true == tpool_add_job(p_tpool, job1, NULL, NULL));
    CU_ASSERT(true == tpool_add_job(p_tpool, job2, NULL, NULL));
    CU_ASSERT(true == tpool_add_job(p_tpool, job1, NULL, NULL));
    CU_ASSERT(true == tpool_add_job(p_tpool, job2, NULL, NULL));
    CU_ASSERT(true == tpool_add_job(p_tpool, job3, NULL, NULL));
    CU_ASSERT(true == tpool_add_job(p_tpool, job3, NULL, NULL));
    CU_ASSERT(true == tpool_add_job(p_tpool, job3, NULL, NULL));
    CU_ASSERT(true == tpool_add_job(p_tpool, job1, NULL, NULL));
    CU_ASSERT(true == tpool_add_job(p_tpool, job2, NULL, NULL));
    CU_ASSERT(true == tpool_add_job(p_tpool, job1, NULL, NULL));
    CU_ASSERT(true == tpool_add_job(p_tpool, job2, NULL, NULL));
    CU_ASSERT(true == tpool_add_job(p_tpool, job1, NULL, NULL));
    CU_ASSERT(true == tpool_add_job(p_tpool, job2, NULL, NULL));
    CU_ASSERT(true == tpool_add_job(p_tpool, job1, NULL, NULL));
    CU_ASSERT(true == tpool_add_job(p_tpool, job2, NULL, NULL));
    CU_ASSERT(true == tpool_add_job(p_tpool, job1, NULL, NULL));
    CU_ASSERT(true == tpool_add_job(p_tpool, job2, NULL, NULL));
    CU_ASSERT(true == tpool_add_job(p_tpool, job1, NULL, NULL));
    CU_ASSERT(true == tpool_add_job(p_tpool, job2, NULL, NULL));
    CU_ASSERT(true == tpool_add_job(p_tpool, job3, NULL, NULL));
    CU_ASSERT(true == tpool_add_job(p_tpool, job3, NULL, NULL));
    CU_ASSERT(true == tpool_add_job(p_tpool, job3, NULL, NULL));

}


void test_tpool_destroy()
{
    CU_ASSERT(true == tpool_destroy(&p_tpool));
}


int main ()
{
    CU_TestInfo suite1_tests[] = {
        {"Testing tpool_create():", test_tpool_create},  

        {"Testing tpool_add_job():", test_tpool_add_job}, 

        {"Testing tpool_destroy():", test_tpool_destroy}, CU_TEST_INFO_NULL
    
    };

    CU_SuiteInfo suites[] = 
    {
        {"Suite-1:", init_suite1, clean_suite1, .pTests = suite1_tests},
        CU_SUITE_INFO_NULL
    };

    if (0 != CU_initialize_registry())
    {
        return CU_get_error();
    }

    if (0 != CU_register_suites(suites))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_basic_show_failures(CU_get_failure_list());
    int num_failed = CU_get_number_of_failures();
    CU_cleanup_registry();
    printf("\n");
    return num_failed;
}