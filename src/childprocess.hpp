#pragma once

// #if __unix__ || __APPLE__
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>

#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

/*
TODO:
  1. Gestire sigchild
  2.    
    2a. Stop using execl and start using execv
    2b. Implement the first constructor
    3c. Implement the other calling the first
*/

/*
Create a sub process and communicate with stdin/stdout
*/
class ChildProcess
{
public:
    // Constructors
    ChildProcess(const std::string, const std::vector<std::string>);
    ChildProcess(const std::string);
    ChildProcess() = delete;

    // Destructor
    ~ChildProcess();

    // Read and write operation
    std::string readFromChild(const int) const;
    void writeToChild(std::string) const;

    // Kill
    void forceKill();

private:
    int m_fdR, m_fdW;  // File descriptors of the pipes
    int m_pidChild;    // Pid of the child process
    bool m_isAlive;    // State of the child process
};

ChildProcess::ChildProcess(const std::string name, const std::vector<std::string> argv)
{
    return;  // TODO
}

ChildProcess::ChildProcess(const std::string name)
{
    int fd1[2];
    int fd2[2];
    pid_t pid;

    // Create pipes
    if ((pipe(fd1) < 0) || (pipe(fd2) < 0))
        throw std::runtime_error("Pipe error");

    // Fork the process
    if ((pid = fork()) < 0)  // ERROR
    {
        throw std::runtime_error("Fork error");
    }
    else  if (pid == 0)  // CHILD PROCESS
    {
        // Close the unused end of the pipes
        close(fd1[1]);
        close(fd2[0]);

        // Redirect stdin
        if (fd1[0] != STDIN_FILENO)
        {
            if (dup2(fd1[0], STDIN_FILENO) != STDIN_FILENO)
                throw std::runtime_error("dup2 error to stdin");

            close(fd1[0]);
        }

        // Redirect stdout
        if (fd2[1] != STDOUT_FILENO)
        {
            if (dup2(fd2[1], STDOUT_FILENO) != STDOUT_FILENO)
                throw std::runtime_error("dup2 error to stdout");

            close(fd2[1]);
        }

        // Launch the other program
        if (execl(name.c_str(), name.c_str(), NULL) < 0)
            throw std::runtime_error("execl error");
    }
    else  // PARENT PROCESS
    {
        // Close the unused end of the pipes...
        close(fd1[0]);
        close(fd2[1]);

        // Setup the member variables
        m_fdR = fd2[0];
        m_fdW = fd1[1];
        m_pidChild = pid;
        m_isAlive = true;
    }
}

ChildProcess::~ChildProcess()
{   
    // Signal the child to terminate
    kill(m_pidChild, SIGTERM);

    // Wait the child to terminate to cleanup the process table
    waitpid(m_pidChild, NULL, 0);
}

std::string ChildProcess::readFromChild(const int count) const
{
    // Create the buffer
    auto buff = std::make_unique<char[]>(count + 1);

    // Read from the pipe
    int rv = read(m_fdR, buff.get(), count);

    // Check if errors occured during the reading
    if (rv == 0)
        throw std::runtime_error("Child closed the pipe"); 
    if (rv == -1)
        throw std::runtime_error("Read error from pipe");

    // Wrap the buffer in a string and return it, removing the \n
    std::string retval(buff.get(), rv - 1);
    return retval;
}

void ChildProcess::writeToChild(std::string str) const
{
    // Append the line feed to the string
    str += '\n';

    // Write the string to the pipe and check if it's successful
    if (write(m_fdW, str.c_str(), str.size()) != str.size())
        throw std::runtime_error("Write error to pipe");
}

void ChildProcess::forceKill()
{
    return;  // TODO
}


//#elif _WIN32
    // Windows.h code
/*
TODO:
  Implementare l'equivalente Windows
*/
//#else
    // NO CODE
//#endif