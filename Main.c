#include "cstring.h"


int main(void)
{
    cstring s = cstr_init();
    cstring str1 = cstr_init("k-hahkj89217");
    cstr_literal(str2, "YOu Beyocchhh");
    cstring tmp = (cstring)str2;

    cstr_push_back(&s, 'A');
    cstr_push_back(&str1, 'B');

    cstr_swap(&str1, &s);

    cstr_println(s);
    cstr_println(str1);
    cstr_println(tmp);
}
