#include "../../cstring.h"
#include "../unity/unity.h"

void test_cstr_shrink_to_fit(void) {
    cstring s = cstr_init(100);
    cstr_assign(&s, "hello");
    TEST_ASSERT_TRUE(cstr_capacity(s) > 5);
    cstr_shrink_to_fit(&s);
    TEST_ASSERT_EQUAL_INT(5, cstr_capacity(s));
    TEST_ASSERT_EQUAL_STRING("hello", s);
    cstr_free(&s);
}
