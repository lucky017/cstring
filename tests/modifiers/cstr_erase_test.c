#include "../unity/unity.h"
#include "../../cstring.h"

void test_cstr_erase(void) {
    cstring s = cstr_init("hello world");
    
    // Erase by index
    cstr_erase(&s, 5, 1);
    TEST_ASSERT_EQUAL_STRING("helloworld", s);
    
    // Erase single char by iterator
    cstr_iterator it = cstr_begin(s);
    it.it += 4; // 'o'
    cstr_erase(&s, it);
    TEST_ASSERT_EQUAL_STRING("hellworld", s);
    
    // Erase range
    cstr_iterator first = cstr_begin(s);
    first.it += 4; // 'w'
    cstr_iterator last = cstr_end(s);
    cstr_erase(&s, first, last);
    TEST_ASSERT_EQUAL_STRING("hell", s);
    
    // Erase everything
    cstr_erase(&s, 0, cstr_size(s));
    TEST_ASSERT_EQUAL_size_t(0, cstr_size(s));
    TEST_ASSERT_EQUAL_STRING("", s);
    
    cstr_free(&s);
}
