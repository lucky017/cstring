#include "../unity/unity.h"
#include "../../cstring.h"

void test_cstr_resize(void) {
    cstring s = cstr_init("abc");
    
    // Grow
    cstr_resize(&s, 5, 'x');
    TEST_ASSERT_EQUAL_STRING("abcxx", s);
    
    // Shrink
    cstr_resize(&s, 2, 'y');
    TEST_ASSERT_EQUAL_STRING("ab", s);
    
    cstr_free(&s);
}
