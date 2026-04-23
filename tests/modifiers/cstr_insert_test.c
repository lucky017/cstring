#include "../../cstring.h"
#include "../unity/unity.h"

void test_cstr_insert(void) {
    cstring s = cstr_init("hello");

    // insert str at index
    cstr_insert(&s, 5, " world");
    TEST_ASSERT_EQUAL_STRING("hello world", s);

    // insert count char at index
    cstr_insert(&s, 0, 3, '!');
    TEST_ASSERT_EQUAL_STRING("!!!hello world", s);

    // insert char at iterator
    cstr_insert(&s, cstr_begin(s), '?');
    TEST_ASSERT_EQUAL_STRING("?!!!hello world", s);

    // insert substring
    cstring s2 = cstr_init("12345");
    cstr_insert(&s, 0, s2, 1, 3); // insert "234"
    TEST_ASSERT_EQUAL_STRING("234?!!!hello world", s);

    cstr_free(&s);
    cstr_free(&s2);
}
