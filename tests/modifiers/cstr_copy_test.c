#include "../unity/unity.h"
#include "../../cstring.h"

void test_cstr_copy(void) {
    cstring s = cstr_init("source");
    cstring dest = cstr_init("______");
    
    size_t copied = cstr_copy(&dest, s, 3, 1); // copy "our"
    TEST_ASSERT_EQUAL_size_t(3, copied);
    TEST_ASSERT_EQUAL_CHAR('o', dest[0]);
    TEST_ASSERT_EQUAL_CHAR('u', dest[1]);
    TEST_ASSERT_EQUAL_CHAR('r', dest[2]);
    
    cstr_free(&s);
    cstr_free(&dest);
}
