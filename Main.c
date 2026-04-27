#include "cstring.h"


int main(void)
{
    cstring str = cstr_init();
    cstr_free(&str);
    return EXIT_SUCCESS;   
}