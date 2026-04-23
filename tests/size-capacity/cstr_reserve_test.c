#include "../../cstring.h"
#include "../unity/unity.h"

void test_cstr_reserve(void) {
    cstring s = cstr_init("test");
    size_t old_cap = cstr_capacity(s);
    cstr_reserve(&s, old_cap + 10);
    TEST_ASSERT_TRUE(cstr_capacity(s) >= old_cap + 10);
    TEST_ASSERT_EQUAL_STRING("test", s);
    cstr_free(&s);
}
