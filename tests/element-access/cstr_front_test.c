#include "../../cstring.h"
#include "../unity/unity.h"

void test_cstr_front(void) {
    cstring s = cstr_init("hello");
    TEST_ASSERT_EQUAL_CHAR('h', cstr_front(s));
    cstr_free(&s);

    cstring s2 = cstr_init("");
    TEST_ASSERT_EQUAL_INT(-1, cstr_front(s2));
    cstr_free(&s2);
}
