#include "../unity/unity.h"
#include "../../cstring.h"

void test_cstr_assign(void) {
    cstring s = cstr_init("initial");
    
    // Assign entire string
    cstr_assign(&s, "new string");
    TEST_ASSERT_EQUAL_STRING("new string", s);
    
    // Assign first n characters
    cstr_assign(&s, "hello world", 5);
    TEST_ASSERT_EQUAL_STRING("hello", s);
    
    // Assign substring
    cstr_assign(&s, "abcdef", 2, 3);
    TEST_ASSERT_EQUAL_STRING("cde", s);
    
    // Assign count copies of char
    cstr_assign(&s, 5, 'x');
    TEST_ASSERT_EQUAL_STRING("xxxxx", s);
    
    cstr_free(&s);
}
