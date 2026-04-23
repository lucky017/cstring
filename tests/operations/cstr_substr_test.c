#include "../unity/unity.h"
#include "../../cstring.h"

void test_cstr_substr(void) {
    cstring s = cstr_init("hello world");
    
    cstring sub = cstr_substr(s, 6, 5);
    TEST_ASSERT_EQUAL_STRING("world", sub);
    
    cstring sub2 = cstr_substr(s, 0, 5);
    TEST_ASSERT_EQUAL_STRING("hello", sub2);
    
    cstr_free(&s);
    cstr_free(&sub);
    cstr_free(&sub2);
}
