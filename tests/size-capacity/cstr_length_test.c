#include "../../cstring.h"
#include "../unity/unity.h"

void test_cstr_length(void) {
    cstring s = cstr_init("hello");
    TEST_ASSERT_EQUAL_INT(5, cstr_length(s));
    cstr_free(&s);
}
