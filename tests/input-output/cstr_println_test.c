#include "../../cstring.h"
#include "../unity/unity.h"

void test_cstr_println(void) {
    // Just verify it doesn't crash, as it prints to stdout
    cstring s = cstr_init("testing println");
    cstr_println(s);
    cstr_free(&s);
}
