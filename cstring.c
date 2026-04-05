
#include "cstring.h"
#include<stddef.h>

typedef struct { char* it; } iterator;

#define distance(start, end) \
    ((ptrdiff_t)((end).it - (start).it))

int main(void)
{
	/* char *s = "KILL"; */
	/* iterator begin = { s }, */
	/* 		 end = { s + strlen(s) }; */
	/* size_t size = strlen(s); */
	/* size_t dist = distance(begin, end); */

	cstring str = NULL;
	cstring_init(&str, "LUKCY");
	size_t dist = cstring_distance(cstring_begin(str + 4), cstring_end(str));
	size_t size = cstring_size(str);
	printf("distance = %lu\nsize = %lu\n", dist, size);
	
}
