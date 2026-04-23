#ifndef ALL_TESTS_H
#define ALL_TESTS_H

// Member Functions
void test_cstr_init(void);
void test_cstr_free(void);
void test_cstr_literal(void);

// Size & Capacity
void test_cstr_size(void);
void test_cstr_length(void);
void test_cstr_capacity(void);
void test_cstr_max_size(void);
void test_cstr_shrink_to_fit(void);
void test_cstr_reserve(void);
void test_cstr_distance(void);

// Input / Output
void test_cstr_getstr(void);
void test_cstr_println(void);

// Element Access
void test_cstr_at(void);
void test_cstr_front(void);
void test_cstr_back(void);
void test_cstr_empty(void);

// Iterators
void test_cstr_iterators(void);
void test_iter_next_prev(void);

// Modifiers
void test_cstr_insert(void);
void test_cstr_erase(void);
void test_cstr_assign(void);
void test_cstr_append(void);
void test_cstr_replace(void);
void test_cstr_clear(void);
void test_cstr_push_back(void);
void test_cstr_pop_back(void);
void test_cstr_copy(void);
void test_cstr_resize(void);
void test_cstr_swap(void);

// Operations
void test_cstr_substr(void);

#endif
