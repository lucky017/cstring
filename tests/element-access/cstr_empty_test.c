#include "../../cstring.h"
#include "../unity/unity.h"

void test_cstr_empty(void) {
    cstring s = cstr_init("");
    TEST_ASSERT_TRUE(cstr_empty(s));
    cstr_free(&s);

    s = cstr_init("not empty");
    TEST_ASSERT_FALSE(cstr_empty(s));
    cstr_free(&s);

    TEST_ASSERT_TRUE(cstr_empty(NULL));
}
