
#include "../../cstring.h"



int main(void)
{
	cstring str;
	cstring_init(&str, 100);
	cstring_assign(&str, "Testing...");
	cstring_reserve(&str, 5);
	printf("Size = %lu\nCap = %lu\n", 
			cstring_size(str),
			cstring_capacity(str));
	return 0;
}
