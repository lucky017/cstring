
#include<iostream>
#include<string>

int main(void)
{
	std::string s = "The Kind of Movie!!";
	s.insert(s.begin()+1, ':');
	std::cout << s << std::endl;
}
