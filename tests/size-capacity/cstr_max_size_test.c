#include "../../cstring.h"
#include "../unity/unity.h"

void test_cstr_max_size(void) {
    size_t ms = cstr_max_size();
    TEST_ASSERT_TRUE(ms > 0);
}
