#include "../../cstring.h"
#include "../unity/unity.h"

void test_cstr_size(void) {
    cstring s = cstr_init("abc");
    TEST_ASSERT_EQUAL_INT(3, cstr_size(s));
    cstr_free(&s);

    s = cstr_init("");
    TEST_ASSERT_EQUAL_INT(0, cstr_size(s));
    cstr_free(&s);

    TEST_ASSERT_EQUAL_INT(0, cstr_size(NULL));
}
