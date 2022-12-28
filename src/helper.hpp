#pragma once

#include <string>
#include <vector>
#include <memory>

/*
Create a sub process and communicate with stdin/stdout
*/
class SubProcess
{
public:
    // Constructors
    SubProcess(const std::string, const std::vector<std::string>);
    SubProcess(const std::string);
    SubProcess() = delete;

    // Destructor
    ~SubProcess();

    // Read and write operation
    std::string sp_read(const int) const;
    void sp_write(std::string) const;

private:
    class State;
    std::unique_ptr<State> m_state;
};
