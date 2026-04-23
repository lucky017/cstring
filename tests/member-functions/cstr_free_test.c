#include "../../cstring.h"
#include "../unity/unity.h"

void test_cstr_free(void) {
    cstring s = cstr_init("to be freed");
    TEST_ASSERT_NOT_NULL(s);
    cstr_free(&s);
    TEST_ASSERT_NULL(s);

    // Freeing a NULL cstring should be safe
    cstring s2 = NULL;
    cstr_free(&s2);
    TEST_ASSERT_NULL(s2);
}
