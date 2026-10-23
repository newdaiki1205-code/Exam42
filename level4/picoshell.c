#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

void fd_cleaner(int *pipefd, int prev_fd)
{
    if(pipefd[0] != -1)
        close(pipefd[0]);
    if(pipefd[1] != -1)
        close(pipefd[1]);
    if(prev_fd != -1)
        close(prev_fd);
}

int    picoshell(char **cmds[])
{
    int pipefd[2];
    int status;
    int i = 0;
    int prev_fd = -1;
    pid_t pid;

    if(!cmds || !cmds[0])
        return 1;

    while(cmds[i])
    {
        // 1. need pipe? = multiple cmds?
        if(cmds[i + 1])
        {
            if(pipe(pipefd) < 0)
                return fd_cleaner(pipefd, prev_fd), 1;
        }
        else
        {
            pipefd[0] = -1;
            pipefd[1] = -1;
        }
        pid = fork();
        if(pid == -1)
            return fd_cleaner(pipefd, prev_fd), 1;
        if (pid == 0)
        {
            //2. where should it read? STDIN or prev_fd?
            if(prev_fd != -1)
            {
                if(dup2(prev_fd, STDIN_FILENO) < 0)
                {
                    fd_cleaner(pipefd, prev_fd);
                    exit(1);
                }
            }
            //3. where should it write? STDOUT or pipefd[1]?
            if(cmds[i + 1])
            {
                if(dup2(pipefd[1], STDOUT_FILENO) < 0)
                {
                    fd_cleaner(pipefd, prev_fd);
                    exit(1);
                }
            }
            if(prev_fd != -1)
                close(prev_fd); //it means close prev_fd = previous pipefd[0]
            //4. no need pipefd in child = assign fd in STDIN and STDOUT
            close(pipefd[0]);
            close(pipefd[1]);
            execvp(cmds[i][0], cmds[i]);
            exit(1);
        }
        //5. Need to pass the output? = pass prev_fd to child
        if(cmds[i + 1])
        {
            if(prev_fd != -1)
                close(prev_fd);
            prev_fd = pipefd[0];
        }
        //fd_cleaner(pipefd, prev_fd); pipefd[0] = prev_fd should not be closed in parent. Otherwise, it cannot access to the file. -> child takes over
        close(pipefd[1]);
        i++;
    }
    while(wait(&status) > 0)
    {
        if(WIFEXITED(status) == 0 || WEXITSTATUS(status) != 0)
            return 1;
    }
    return 0;
}

#include <stdio.h>
int main(void)
{
    char *cmd1[] = {"/bin/ls", "level-1", NULL};
    // char *cmd1[] = {"/bin/ls", NULL};
    //char *cmd2[] = {"/usr/bin/grep", "picoshell", NULL};
    char *cmd2[] = {"cat", NULL};
    char **cmds[] = {cmd1, cmd2, NULL};

    int result = picoshell(cmds);
    printf("picoshell returned %d\n", result);

    return 0;
}