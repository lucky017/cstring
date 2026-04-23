#include "../../cstring.h"
#include "../unity/unity.h"

void test_iter_next_prev(void) {
    cstring s = cstr_init("abc");

    // Normal iterator
    cstr_iterator it = cstr_begin(s);
    iter_next(&it);
    TEST_ASSERT_EQUAL_CHAR('b', *it.it);
    iter_prev(&it);
    TEST_ASSERT_EQUAL_CHAR('a', *it.it);

    // Const iterator
    cstr_const_iterator cit = cstr_cbegin(s);
    iter_next(&cit);
    TEST_ASSERT_EQUAL_CHAR('b', *cit.it);
    iter_prev(&cit);
    TEST_ASSERT_EQUAL_CHAR('a', *cit.it);

    // Reverse iterator
    cstr_reverse_iterator rit = cstr_rbegin(s);
    iter_next(&rit);
    TEST_ASSERT_EQUAL_CHAR('b', *rit.it);
    iter_prev(&rit);
    TEST_ASSERT_EQUAL_CHAR('c', *rit.it);

    // Const reverse iterator
    cstr_const_reverse_iterator crit = cstr_crbegin(s);
    iter_next(&crit);
    TEST_ASSERT_EQUAL_CHAR('b', *crit.it);
    iter_prev(&crit);
    TEST_ASSERT_EQUAL_CHAR('c', *crit.it);

    cstr_free(&s);
}
