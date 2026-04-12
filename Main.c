#include "cstring.h"

int main(void)
{
    cstring str = cstr_init("");
    cstring other = cstr_init("");
    

    cstr_swap(&str, &other);

    printf("str = %s ==&== Size = %lu\n", str, cstr_size(str));
    printf("other = %s ==&== Size = %lu\n", other, cstr_size(other));
}