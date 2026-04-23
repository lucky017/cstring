#include "../unity/unity.h"
#include "../../cstring.h"

void test_cstr_replace(void) {
    cstring s = cstr_init("hello world");
    
    // replace [first,last) with count2 copies of ch
    cstr_replace(&s, cstr_cbegin(s), cstr_cbegin(s + 5), 2, 'X');
    TEST_ASSERT_EQUAL_STRING("XX world", s);
    
    // replace [pos,pos+count) with first count2 chars of other
    cstr_replace(&s, 3, 5, "ABCDE", 3);
    TEST_ASSERT_EQUAL_STRING("XX ABC", s);
    
    // replace [pos,pos+count) with other[pos2:pos2+count2)
    cstr_replace(&s, 0, 2, "12345", 1, 3);
    TEST_ASSERT_EQUAL_STRING("234 ABC", s);
    
    cstr_free(&s);
}
