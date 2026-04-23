#include "../../cstring.h"
#include "../unity/unity.h"

void test_cstr_capacity(void) {
    cstring s = cstr_init(10);
    TEST_ASSERT_TRUE(cstr_capacity(s) >= 10);
    cstr_free(&s);
}
