#include "../unity/unity.h"
#include "../../cstring.h"

void test_cstr_append(void) {
    cstring s = cstr_init("hello");
    
    // Append full string
    cstr_append(&s, " world");
    TEST_ASSERT_EQUAL_STRING("hello world", s);
    
    // Append count chars
    cstr_append(&s, "!!!123", 3);
    TEST_ASSERT_EQUAL_STRING("hello world!!!", s);
    
    // Append count copies of char
    cstr_append(&s, 2, '.');
    TEST_ASSERT_EQUAL_STRING("hello world!!!..", s);
    
    // Append substring
    cstr_append(&s, " --append-- ", 3, 7);
    TEST_ASSERT_EQUAL_STRING("hello world!!!..append-", s);
    
    cstr_free(&s);
}
