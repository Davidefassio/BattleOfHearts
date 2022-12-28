#include "helper.hpp"

#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdexcept>

#if __unix__ || __APPLE__
    // unistd.h code
#elif _WIN32
    // Windows.h code
#else
    // NO CODE
#endif

/*
TODO:
    Rendere la libreria compatibile con windows e macos
    Usare le guard come sopra, ripensare cosa SubProcess mantiene come stato.
*/

/*
TODO:
    Gestire sigchild
    avere flag in subprocess per vedere se il figlio e' morto, usarlo per le r/w e distruttore
    Se il figlio e' morto e non c'e niente da leggere distruggere l'oggetto
    se il figlio e' morto impedire write
*/

/*
TODO:
  1. Stop using execl and start using execv
  2. Implement the first constructor
  3. Implement the other calling the first
*/
class SubProcess::State
{
public:
    State() = default;
    State(int r, int w, int p): fd_r(r), fd_w(w), pid_child(p) {}

    int fd_r, fd_w;
    int pid_child;
};

SubProcess::SubProcess(const std::string name, const std::vector<std::string> argv)
{
    return;  // TODO
}

SubProcess::SubProcess(const std::string name)
{
    int fd1[2];
    int fd2[2];
    pid_t pid;

    // Create pipes
    if ((pipe(fd1) < 0) || (pipe(fd2) < 0))
        throw std::runtime_error("Pipe error");

    // Fork the process
    if ((pid = fork()) < 0)
    {
        throw std::runtime_error("Fork error");
    }
    else  if (pid == 0)     // CHILD PROCESS
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

        m_state = std::make_unique<State>(fd2[0], fd1[1], pid);
    }
}

SubProcess::~SubProcess()
{   
    // Signal the child to terminate
    kill(m_state->pid_child, SIGTERM);

    // Wait the child to terminate to cleanup the process table
    waitpid(m_state->pid_child, NULL, 0);
}

std::string SubProcess::sp_read(const int count) const
{
    // Create the buffer
    auto buff = std::make_unique<char[]>(count + 1);

    // Read from the pipe
    int rv = read(m_state->fd_r, buff.get(), count);

    // Check if errors occured during the reading
    if (rv == 0)
        throw std::runtime_error("Child closed the pipe"); 
    if (rv == -1)
        throw std::runtime_error("Read error from pipe");

    // Wrap the buffer in a string and return it
    std::string retval(buff.get(), rv);
    return retval;
}

void SubProcess::sp_write(std::string str) const
{
    // Append the line feed to the string
    str += '\n';

    // Write the string to the pipe and check if it's successful
    if (write(m_state->fd_w, str.c_str(), str.size()) != str.size())
        throw std::runtime_error("Write error to pipe");
}
