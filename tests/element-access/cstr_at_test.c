#include "../../cstring.h"
#include "../unity/unity.h"

void test_cstr_at(void) {
    cstring s = cstr_init("hello");
    TEST_ASSERT_EQUAL_CHAR('h', cstr_at(s, 0));
    TEST_ASSERT_EQUAL_CHAR('e', cstr_at(s, 1));
    TEST_ASSERT_EQUAL_CHAR('o', cstr_at(s, 4));
    TEST_ASSERT_EQUAL_INT(-1, cstr_at(s, 5));
    cstr_free(&s);
}
