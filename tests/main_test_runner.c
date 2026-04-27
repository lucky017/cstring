#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include "cmocka.h"
#include "all_tests.h"

/* 
 * Dummy test implementations since the actual .c files in subdirectories 
 * were empty or missing. You should implement your tests in those files.
 */

// Example of how a cmocka test looks
void test_example(void **state) {
    (void) state;
    assert_int_equal(0, 0);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_example),
        // Add your tests here, e.g.:
        // cmocka_unit_test(test_cstr_init),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
