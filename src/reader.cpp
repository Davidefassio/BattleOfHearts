#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
	std::string str;
	std::cin >> str;

	std::cout << "PROC: reader + " << str << std::flush;

	return 0;
}
