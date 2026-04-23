#include "../unity/unity.h"
#include "../../cstring.h"

void test_cstr_clear(void) {
    cstring s = cstr_init("content");
    TEST_ASSERT_FALSE(cstr_empty(s));
    
    cstr_clear(&s);
    TEST_ASSERT_TRUE(cstr_empty(s));
    TEST_ASSERT_EQUAL_size_t(0, cstr_size(s));
    
    cstr_free(&s);
}
