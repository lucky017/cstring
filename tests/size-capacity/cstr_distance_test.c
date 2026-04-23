#include "../../cstring.h"
#include "../unity/unity.h"

void test_cstr_distance(void) {
    cstring s = cstr_init("hello");
    cstr_iterator begin = cstr_begin(s);
    cstr_iterator end = cstr_end(s);
    TEST_ASSERT_EQUAL_INT(5, cstr_distance(begin, end));
    
    cstr_reverse_iterator rbegin = cstr_rbegin(s);
    cstr_reverse_iterator rend = cstr_rend(s);
    TEST_ASSERT_EQUAL_INT(5, cstr_distance(rbegin, rend));
    
    cstr_free(&s);
}
