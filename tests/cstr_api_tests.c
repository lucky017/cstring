#include "unity/unity.h"
#include "../cstring.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

void setUp(void) {}
void tearDown(void) {}

static void test_cstr_literal_init_and_size_api(void)
{
    cstr_literal(lit, "hello");
    TEST_ASSERT_EQUAL_STRING("hello", lit);

    cstring s = cstr_init("hello");
    TEST_ASSERT_NOT_NULL(s);
    TEST_ASSERT_EQUAL_size_t(5, cstr_size(s));
    TEST_ASSERT_EQUAL_size_t(5, cstr_length(s));
    TEST_ASSERT_GREATER_OR_EQUAL_size_t(5, cstr_capacity(s));
    TEST_ASSERT_FALSE(cstr_empty(s));
    TEST_ASSERT_EQUAL_size_t((size_t)-1, cstr_max_size());
    cstr_free(&s);
    TEST_ASSERT_NULL(s);
}

static void test_cstr_capacity_controls(void)
{
    cstring s = cstr_init("abc");
    size_t min_cap = cstr_capacity(s) + 10;

    cstr_reserve(&s, min_cap);
    TEST_ASSERT_GREATER_OR_EQUAL_size_t(min_cap, cstr_capacity(s));

    cstr_shrink_to_fit(&s);
    TEST_ASSERT_EQUAL_size_t(cstr_size(s), cstr_capacity(s));

    cstr_free(&s);
}

static void test_cstr_element_access(void)
{
    cstring s = cstr_init("world");

    TEST_ASSERT_EQUAL_CHAR('o', cstr_at(s, 1));
    TEST_ASSERT_EQUAL_CHAR((char)-1, cstr_at(s, 99));
    TEST_ASSERT_EQUAL_CHAR('w', cstr_front(s));
    TEST_ASSERT_EQUAL_CHAR('d', cstr_back(s));

    cstr_free(&s);
}

static void test_cstr_iterators_and_distance(void)
{
    cstring s = cstr_init("abcd");

    cstr_iterator begin = cstr_begin(s);
    cstr_const_iterator cbegin = cstr_cbegin(s);
    cstr_iterator end = cstr_end(s);
    cstr_const_iterator cend = cstr_cend(s);
    cstr_reverse_iterator rbegin = cstr_rbegin(s);
    cstr_const_reverse_iterator crbegin = cstr_crbegin(s);
    cstr_reverse_iterator rend = cstr_rend(s);
    cstr_const_reverse_iterator crend = cstr_crend(s);

    TEST_ASSERT_EQUAL_PTR(s, begin.it);
    TEST_ASSERT_EQUAL_PTR(s, cbegin.it);
    TEST_ASSERT_EQUAL_PTR(s + 4, end.it);
    TEST_ASSERT_EQUAL_PTR(s + 4, cend.it);
    TEST_ASSERT_EQUAL_PTR(s + 3, rbegin.it);
    TEST_ASSERT_EQUAL_PTR(s + 3, crbegin.it);
    TEST_ASSERT_EQUAL_PTR(s - 1, rend.it);
    TEST_ASSERT_EQUAL_PTR(s - 1, crend.it);
    TEST_ASSERT_EQUAL_size_t(4, cstr_distance(begin, end));
    TEST_ASSERT_EQUAL_size_t(4, cstr_distance(rbegin, rend));

    cstr_free(&s);
}

static void test_cstr_insert_erase_and_clear(void)
{
    cstring s = cstr_init("abcd");

    cstr_insert(&s, 2, 1, 'X');
    TEST_ASSERT_EQUAL_STRING("abXcd", s);

    cstr_insert(&s, 5, "YZ");
    TEST_ASSERT_EQUAL_STRING("abXcdYZ", s);

    cstr_erase(&s, 2, 1);
    TEST_ASSERT_EQUAL_STRING("abcdYZ", s);

    cstr_iterator first = cstr_begin(s);
    cstr_iterator last = cstr_end(s);
    first.it += 4;
    cstr_erase(&s, first, last);
    TEST_ASSERT_EQUAL_STRING("abcd", s);

    cstr_clear(&s);
    TEST_ASSERT_TRUE(cstr_empty(s));
    TEST_ASSERT_EQUAL_size_t(0, cstr_size(s));

    cstr_free(&s);
}

static void test_cstr_assign_append_and_replace(void)
{
    cstring s = cstr_init();

    cstr_assign(&s, "cat");
    TEST_ASSERT_EQUAL_STRING("cat", s);

    cstr_assign(&s, 3, 'x');
    TEST_ASSERT_EQUAL_STRING("xxx", s);

    cstr_append(&s, "yz");
    TEST_ASSERT_EQUAL_STRING("xxxyz", s);

    cstr_append(&s, 2, '!');
    TEST_ASSERT_EQUAL_STRING("xxxyz!!", s);

    cstr_append(&s, "ABCDE", 1, 3);
    TEST_ASSERT_EQUAL_STRING("xxxyz!!BCD", s);

    cstr_replace(&s, 0, 2, 3, '*');
    TEST_ASSERT_EQUAL_STRING("***xyz!!BCD", s);

    cstr_replace(&s, 0, 3, "hello", 5);
    TEST_ASSERT_EQUAL_STRING("helloxyz!!BCD", s);

    cstr_replace(&s, 0, 5, "12345", 1, 3);
    TEST_ASSERT_EQUAL_STRING("234xyz!!BCD", s);

    cstr_free(&s);
}

static void test_cstr_push_pop_resize_swap_substr_and_copy(void)
{
    cstring s = cstr_init("ab");
    cstring other = cstr_init("zzz");

    cstr_push_back(&s, 'c');
    TEST_ASSERT_EQUAL_STRING("abc", s);

    cstr_pop_back(&s);
    TEST_ASSERT_EQUAL_STRING("ab", s);

    cstr_resize(&s, 4, 'x');
    TEST_ASSERT_EQUAL_STRING("abxx", s);

    cstr_resize(&s, 1, 0);
    TEST_ASSERT_EQUAL_STRING("a", s);

    cstr_swap(&s, &other);
    TEST_ASSERT_EQUAL_STRING("zzz", s);
    TEST_ASSERT_EQUAL_STRING("a", other);

    cstring sub = cstr_substr(s, 1, 2);
    TEST_ASSERT_EQUAL_STRING("zz", sub);

    cstring dest = cstr_init(5, '_');
    TEST_ASSERT_EQUAL_size_t(3, cstr_copy(&dest, s, 3, 0));
    TEST_ASSERT_EQUAL_CHAR('z', dest[0]);
    TEST_ASSERT_EQUAL_CHAR('z', dest[1]);
    TEST_ASSERT_EQUAL_CHAR('z', dest[2]);

    cstr_free(&dest);
    cstr_free(&sub);
    cstr_free(&s);
    cstr_free(&other);
}

static void test_cstr_stream_io(void)
{
    FILE *in = tmpfile();
    TEST_ASSERT_NOT_NULL(in);
    fputs("warp", in);
    rewind(in);

    cstring s = cstr_init();
    cstr_getstr(&s, 4, in);
    TEST_ASSERT_EQUAL_STRING("warp", s);
    fclose(in);

    FILE *capture = tmpfile();
    TEST_ASSERT_NOT_NULL(capture);
    int stdout_fd = dup(fileno(stdout));
    TEST_ASSERT_TRUE(stdout_fd >= 0);

    fflush(stdout);
    TEST_ASSERT_TRUE(dup2(fileno(capture), fileno(stdout)) >= 0);
    cstr_println(s);
    fflush(stdout);
    TEST_ASSERT_TRUE(dup2(stdout_fd, fileno(stdout)) >= 0);
    close(stdout_fd);

    rewind(capture);
    char out[32] = {0};
    fread(out, 1, sizeof(out) - 1, capture);
    TEST_ASSERT_EQUAL_STRING("warp\n", out);

    fclose(capture);
    cstr_free(&s);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_cstr_literal_init_and_size_api);
    RUN_TEST(test_cstr_capacity_controls);
    RUN_TEST(test_cstr_element_access);
    RUN_TEST(test_cstr_iterators_and_distance);
    RUN_TEST(test_cstr_insert_erase_and_clear);
    RUN_TEST(test_cstr_assign_append_and_replace);
    RUN_TEST(test_cstr_push_pop_resize_swap_substr_and_copy);
    RUN_TEST(test_cstr_stream_io);
    return UNITY_END();
}
