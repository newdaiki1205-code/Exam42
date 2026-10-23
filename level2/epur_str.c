#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int ac, char* av[])
{   
    int i = 0;

    if(ac != 2 || !av[1])
        return ('\n');
    while(av[1][i] == ' ')
        i++;
    while(av[1][i])
    {
        if(av[1][i] != ' ')
            write(1, &av[1][i], 1);
        // else if(av[1][i]  == ' ' && av[1][i - 1] != ' ' && av[1][i + 1] != ' ')
        //     write(1, &av[1][i], 1);
        else if(av[1][i]  == ' ' && av[1][i + 1] != ' ' && av[1][i + 1])
            write(1, &av[1][i], 1);
        i++;
    }
    write(1, "\n", 1);
    return 0;
}