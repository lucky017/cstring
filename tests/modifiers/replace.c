
#include "../../cstring.h"

int main(void)
{
	cstring str;
	cstring_init(&str, 40);
	cstring_assign(&str, "The stunt pulled by REDBULL :)");
	intl_replace_cnt_offset_with_str(&str, 0, 100, "Killin");
	cstring_println(str);
}
