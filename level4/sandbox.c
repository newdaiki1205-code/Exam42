#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <sys/errno.h>

volatile sig_atomic_t g_child_pid = -1;

void alarm_handler(int sig)
{
    (void)sig;
    if(g_child_pid > 0)
        kill((pid_t)g_child_pid, SIGKILL);
}

int sandbox(void (*f)(void), unsigned int timeout, bool verbose)
{
    pid_t pid;
    int status;
    struct sigaction sa;
    
    if(!f || !timeout)
        return -1;

    pid = fork();
    if(pid < 0)
        return -1;
    if(pid == 0)
    {
        f();
        exit(0);
    }
    else
    {
        g_child_pid = pid;

        sigemptyset(&sa.sa_mask);
        sa.sa_flags = 0;
        sa.sa_handler = alarm_handler;
        sigaction(SIGALRM, &sa, NULL);

        alarm(timeout);

        if(waitpid(pid, &status, 0) == -1)
        {
            if(errno == EINTR) //if waitpid is interrupted by SIG, it becomes zombi
            {
                //kill(pid, SIGKILL) not need g_val
                waitpid(pid, NULL, 0); //thats why need to call waitpid again
                if(verbose)
                    printf("Bad function: timed out after %u seconds\n", timeout);
                return 0;
            }
            return -1;
        }

        alarm(0);
        
        if(WIFEXITED(status))
        {
            if(WEXITSTATUS(status) == 0)
            {
                if(verbose)
                    printf("Nice function!\n");
                return 1;
            }
            else
            {
                if(verbose)
                    printf("Bad function: exited with code %d\n", WEXITSTATUS(status));
                return 0;
            }
        }
        if(WIFSIGNALED(status))
        {
            if(verbose)
                printf("Bad function: %s\n", strsignal(WTERMSIG(status)));
            return 0;
        }
    }
    return -1;
}

