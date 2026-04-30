#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include "cmocka.h"
#include "all_tests.h"


void test_example(void **state) {
    (void) state;
    assert_int_equal(0, 0);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_example)
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
