#include "unity/unity.h"
#include "all_tests.h"

void setUp(void) {}
void tearDown(void) {}

int main(void) {
    UNITY_BEGIN();

    // Member Functions
    RUN_TEST(test_cstr_init);
    RUN_TEST(test_cstr_free);
    RUN_TEST(test_cstr_literal);

    // Size & Capacity
    RUN_TEST(test_cstr_size);
    RUN_TEST(test_cstr_length);
    RUN_TEST(test_cstr_capacity);
    RUN_TEST(test_cstr_max_size);
    RUN_TEST(test_cstr_shrink_to_fit);
    RUN_TEST(test_cstr_reserve);
    RUN_TEST(test_cstr_distance);

    // Input / Output
    RUN_TEST(test_cstr_getstr);
    RUN_TEST(test_cstr_println);

    // Element Access
    RUN_TEST(test_cstr_at);
    RUN_TEST(test_cstr_front);
    RUN_TEST(test_cstr_back);
    RUN_TEST(test_cstr_empty);

    // Iterators
    RUN_TEST(test_cstr_iterators);
    RUN_TEST(test_iter_next_prev);

    // Modifiers
    RUN_TEST(test_cstr_insert);
    RUN_TEST(test_cstr_erase);
    RUN_TEST(test_cstr_assign);
    RUN_TEST(test_cstr_append);
    RUN_TEST(test_cstr_replace);
    RUN_TEST(test_cstr_clear);
    RUN_TEST(test_cstr_push_back);
    RUN_TEST(test_cstr_pop_back);
    RUN_TEST(test_cstr_copy);
    RUN_TEST(test_cstr_resize);
    RUN_TEST(test_cstr_swap);

    // Operations
    RUN_TEST(test_cstr_substr);

    return UNITY_END();
}
