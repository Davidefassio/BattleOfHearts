#include <iostream>
#include <chrono>

#include "../lib/tiny-process/process.hpp"

using namespace TinyProcess;

namespace
{
    using namespace std::chrono;

    class Timer
    {
    public:
        Timer() : m_start(m_start = steady_clock::now()) {}

        ~Timer()
        {
            std::cout << Timer::durationInSeconds(steady_clock::now() - m_start) << "s" << std::endl;
        }

        // Utility: convert the duration count in seconds
        static inline double durationInSeconds(duration<double> dur)
        {
            return decltype(dur)::period::num * dur.count() / (double)decltype(dur)::period::den;
        }

    private:
        time_point<steady_clock> m_start;
    };
}

int main(int argc, char *argv[])
{
    // C:\\Users\\Utente\\Desktop\\SoftHeart\\out\\build\\x64-Debug\\SoftHeart.exe
    // C:\\Users\\Utente\\Desktop\\receiver\\main.exe
    Process proc(L"C:\\Users\\Utente\\Desktop\\SoftHeart\\out\\build\\x64-Debug\\SoftHeart.exe", L"", [](const char *bytes, size_t n)
                 { std::cout << "Proc: " << std::string(bytes, n) << std::endl; });

    std::cout << "This: u3tp-simple\n";
    proc.write("u3tp-simple\n");
    proc.write("go\n");
    proc.write("quit\n");

    auto exit_status = proc.get_exit_status();
    std::cout << "Example 1 process returned: " << exit_status << " (" << (exit_status == 0 ? "success" : "failure") << ")" << std::endl;

    /*
    output = child.readFromChild(100);
    std::cout << output << std::endl;

    if (output != "u3tp-simpleok")
        return 0;

    input = "setconstraint -time move 5s";
    std::cout << input << std::endl;
    child.writeToChild(input);

    for (int i = 0; i < 5; ++i)
    {
        {
            Timer t;
            input = "go";
            std::cout << input << std::endl;
            child.writeToChild(input);

            output = child.readFromChild(100);
            std::cout << output << std::endl;
        }

        input = "move ";
        input.push_back(output[0]);
        input.push_back(output[1]);
        std::cout << input << std::endl;
        child.writeToChild(input);
    }

    input = "quit";
    std::cout << input << std::endl;
    child.writeToChild(input);*/

    return 0;
}
