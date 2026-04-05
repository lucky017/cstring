
#include "../../cstring.h"

int main(void)
{
	cstring str;
	cstring_init(&str, 40);
	cstring_assign(&str, "The stunt pulled by REDBULL :)");
	str = intl_insert_str_at_idx(str, 0, "KINDOF");
	cstring substr = cstring_substr(str, 3, 10);
	cstring_println(str);
	cstring_println(substr);
	printf("Distance = %lu\n", 
			cstring_distance(cstring_begin(str).it, cstring_end(substr).it));
}
