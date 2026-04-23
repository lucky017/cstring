#include "../../cstring.h"
#include "../unity/unity.h"

void test_cstr_init(void) {
    // Default init
    cstring s1 = cstr_init();
    TEST_ASSERT_NOT_NULL(s1);
    TEST_ASSERT_EQUAL_INT(0, cstr_size(s1));
    cstr_free(&s1);

    // Size init
    cstring s2 = cstr_init(10);
    TEST_ASSERT_NOT_NULL(s2);
    TEST_ASSERT_EQUAL_INT(10, cstr_size(s2));
    for(int i=0; i<10; i++) TEST_ASSERT_EQUAL_CHAR(' ', s2[i]);
    cstr_free(&s2);

    // Copy init from literal
    cstring s3 = cstr_init("hello");
    TEST_ASSERT_NOT_NULL(s3);
    TEST_ASSERT_EQUAL_STRING("hello", s3);
    TEST_ASSERT_EQUAL_INT(5, cstr_size(s3));
    cstr_free(&s3);

    // Fill init
    cstring s4 = cstr_init(5, 'a');
    TEST_ASSERT_NOT_NULL(s4);
    TEST_ASSERT_EQUAL_STRING("aaaaa", s4);
    TEST_ASSERT_EQUAL_INT(5, cstr_size(s4));
    cstr_free(&s4);

    // Range init
    cstring base = cstr_init("world");
    cstring s5 = cstr_init(cstr_begin(base), cstr_end(base));
    TEST_ASSERT_NOT_NULL(s5);
    TEST_ASSERT_EQUAL_STRING("world", s5);
    cstr_free(&base);
    cstr_free(&s5);

    // Substring init
    cstring base2 = cstr_init("hello world");
    cstring s6 = cstr_init(base2, 6, 5);
    TEST_ASSERT_NOT_NULL(s6);
    TEST_ASSERT_EQUAL_STRING("world", s6);
    cstr_free(&base2);
    cstr_free(&s6);
}
