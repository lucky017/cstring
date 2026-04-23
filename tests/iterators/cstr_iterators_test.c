#include "../../cstring.h"
#include "../unity/unity.h"

void test_cstr_iterators(void) {
    cstring s = cstr_init("abc");

    // begin / end
    cstr_iterator begin = cstr_begin(s);
    cstr_iterator end = cstr_end(s);
    TEST_ASSERT_EQUAL_PTR(s, begin.it);
    TEST_ASSERT_EQUAL_PTR(s + 3, end.it);
    TEST_ASSERT_EQUAL_CHAR('a', *begin.it);

    // cbegin / cend
    cstr_const_iterator cbegin = cstr_cbegin(s);
    cstr_const_iterator cend = cstr_cend(s);
    TEST_ASSERT_EQUAL_PTR(s, cbegin.it);
    TEST_ASSERT_EQUAL_PTR(s + 3, cend.it);
    TEST_ASSERT_EQUAL_CHAR('a', *cbegin.it);

    // rbegin / rend
    cstr_reverse_iterator rbegin = cstr_rbegin(s);
    cstr_reverse_iterator rend = cstr_rend(s);
    TEST_ASSERT_EQUAL_PTR(s + 2, rbegin.it);
    TEST_ASSERT_EQUAL_PTR(s - 1, rend.it);
    TEST_ASSERT_EQUAL_CHAR('c', *rbegin.it);

    // crbegin / crend
    cstr_const_reverse_iterator crbegin = cstr_crbegin(s);
    cstr_const_reverse_iterator crend = cstr_crend(s);
    TEST_ASSERT_EQUAL_PTR(s + 2, crbegin.it);
    TEST_ASSERT_EQUAL_PTR(s - 1, crend.it);
    TEST_ASSERT_EQUAL_CHAR('c', *crbegin.it);

    cstr_free(&s);
}
