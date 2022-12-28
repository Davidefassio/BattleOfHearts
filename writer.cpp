#include <iostream>
#include <unistd.h>
#include "helper.hpp"

int main(int argc, char *argv[])
{
    SubProcess proc("./reader");

    proc.sp_write("ciao");

    std::string out = proc.sp_read(100);

    std::cout << out << "\n";

    return 0;
}