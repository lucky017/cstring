#include "../unity/unity.h"
#include "../../cstring.h"

void test_cstr_swap(void) {
    cstring s1 = cstr_init("one");
    cstring s2 = cstr_init("two");
    
    cstr_swap(&s1, &s2);
    TEST_ASSERT_EQUAL_STRING("two", s1);
    TEST_ASSERT_EQUAL_STRING("one", s2);
    
    cstr_free(&s1);
    cstr_free(&s2);
}
