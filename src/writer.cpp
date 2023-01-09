#include <iostream>
#include <unistd.h>
#include <chrono>

#include "helper.hpp"

namespace
{
    using namespace std::chrono;

    class Timer
    {
    public:
        Timer() : m_start(m_start = steady_clock::now()) {}

        ~Timer()
        {
            std::cout << durationInSeconds(steady_clock::now() - m_start) << "s" << std::endl;
        }

        // Utility: convert the duration count in seconds
        static inline double durationInSeconds(duration<double> dur)
        {
            return decltype(dur)::period::num * dur.count() / (double) decltype(dur)::period::den;
        }

    private:
        time_point<steady_clock> m_start;
    };
}

int main(int argc, char *argv[])
{
    SubProcess proc("./SoftHeart");

    std::string input, output;

    input = "u3tp-simple";
    std::cout << input << std::endl;
    proc.sp_write(input);
    
    output = proc.sp_read(100);
    std::cout << output << std::endl;

    if(output != "u3tp-simpleok")
        return 0;
    
    input = "setconstraint -time move 5s";
    std::cout << input << std::endl;
    proc.sp_write(input);

    for(int i = 0 ; i < 5; ++i)
    {
        {
            Timer t;
            input = "go";
            std::cout << input << std::endl;
            proc.sp_write(input);

            output = proc.sp_read(100);
            std::cout << output << std::endl;
        }

        input = "move ";
        input.push_back(output[0]);
        input.push_back(output[1]);
        std::cout << input << std::endl;
        proc.sp_write(input);
    }

    input = "quit";
    std::cout << input << std::endl;
    proc.sp_write(input);

    return 0;
}
