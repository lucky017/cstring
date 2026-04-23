#include "../../cstring.h"
#include "../unity/unity.h"

void test_cstr_getstr(void) {
    // Use a temporary file to test cstr_getstr
    FILE* tmp = tmpfile();
    fputs("hello world", tmp);
    rewind(tmp);

    cstring s = cstr_init();
    cstr_getstr(&s, 5, tmp);
    TEST_ASSERT_EQUAL_STRING("hello", s);
    TEST_ASSERT_EQUAL_INT(5, cstr_size(s));

    cstr_getstr(&s, 6, tmp);
    TEST_ASSERT_EQUAL_STRING(" world", s);
    TEST_ASSERT_EQUAL_INT(6, cstr_size(s));

    fclose(tmp);
    cstr_free(&s);
}
