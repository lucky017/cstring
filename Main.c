#include "cstring.h"

int main(void)
{
    cstring str = cstr_init("TEK KNIGHT");
    cstr_insert(&str, 4, "KING");

    cstring str_n = intl_assign_str(&str, "The FrenchMan, The Female");
    cstr_println(str_n);
    printf("Capacity of str = %lu\n", cstr_capacity(str_n));
}