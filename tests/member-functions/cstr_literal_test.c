#include "../../cstring.h"
#include "../unity/unity.h"

void test_cstr_literal(void) {
    cstr_literal(s, "hello world");
    TEST_ASSERT_EQUAL_STRING("hello world", s);
    TEST_ASSERT_EQUAL_INT(11, cstr_size(s));
    TEST_ASSERT_FALSE(pvt_is_malloced_(s));
}
