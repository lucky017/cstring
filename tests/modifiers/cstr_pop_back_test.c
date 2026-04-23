#include "../unity/unity.h"
#include "../../cstring.h"

void test_cstr_pop_back(void) {
    cstring s = cstr_init("abc");
    
    cstr_pop_back(&s);
    TEST_ASSERT_EQUAL_STRING("ab", s);
    
    cstr_pop_back(&s);
    TEST_ASSERT_EQUAL_STRING("a", s);
    
    cstr_free(&s);
}
