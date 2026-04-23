#include "../unity/unity.h"
#include "../../cstring.h"

void test_cstr_push_back(void) {
    cstring s = cstr_init("abc");
    
    cstr_push_back(&s, 'd');
    TEST_ASSERT_EQUAL_STRING("abcd", s);
    
    cstr_push_back(&s, 'e');
    TEST_ASSERT_EQUAL_STRING("abcde", s);
    
    cstr_free(&s);
}
