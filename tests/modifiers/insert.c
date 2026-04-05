
#include "../../cstring.h"

int main(void)
{
	cstring str;
	cstring_init(&str, 40);
	cstring_assign(&str, "The stunt pulled by REDBULL :)");
	str = intl_insert_cnt_of_str_at_idx(str, 0, 3, "YAY!!");	
	cstring_println(str);
}
