#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 100
#endif

int ft_strlen(char *s)
{
    int i = 0;

    if(!s)
        return 0;
    while(s[i])
        i++;
    return i;
}

void rewrite(char *s, int len)
{
    int i = 0;

    while(i < len)
    {
        *s = 'x';
        s++;
        i++;
    }
}

void filter(char *input, char *hey, int re_ret)
{
    int i = 0;
    int j = 0;
    int hey_len = ft_strlen(hey);

    while(i < re_ret)
    {
        j = 0;
        if(input[i] == hey[j])
        {
            while(j < hey_len)
            {
                if(input[i + j] != hey[j])
                    break;
                j++;
            }
            if(j == hey_len)
                rewrite(&input[i], hey_len);
        }
        if(j == hey_len)
            i += j;
        else
            i++;
    }
}

int use_realloc(char **buffer)
{
    int re_ret;
    int start;
    int i;
    char *input = (char *)malloc(BUFFER_SIZE * sizeof(char));
    int total_read = 0;
    
    while((re_ret = read(0, input, BUFFER_SIZE)) > 0)
    {
        i = 0;
        total_read += re_ret;
        *buffer = realloc(*buffer, re_ret);
        start = total_read - re_ret;
        while(start < total_read)
        {
            (*buffer)[start] = input[i];
            i++;
            start++;
        }
        free(input);
        input = (char *)calloc(BUFFER_SIZE, sizeof(char));
    }
    if(*input)
        free(input);
    return total_read;
}

int str_append_mem(char **s1, char *s2)
{
    int size1;
    int size2;
    char *tmp;
    
    if(s1 && *s1)
        size1 = ft_strlen(*s1);
    else 
        size1 = 0;
    size2 = ft_strlen(s2);
    tmp = (char*)malloc(sizeof(char) * (size1 + size2 + 1));
    if(!tmp)
        return -1;
    if(s1 && *s1)
        memcpy(tmp, *s1, size1);
    memcpy(tmp + size1, s2, size2);
    tmp[size1 + size2] = '\0';
    if(s1 && *s1)
        free(*s1);
    *s1 = tmp;
    return 0;
}

int prep_buf(char **s1)
{
    int read_re;
    int check;
    char *s2;

    s2 = (char *)malloc(sizeof(char) * BUFFER_SIZE);
    if(!s2)
        return -1;
    while((read_re = read(0, s2, BUFFER_SIZE)) > 0)
    {
        check = str_append_mem(s1, s2);
        if(check < 0)
            return -1;
        free(s2);
        s2 = (char*)calloc(BUFFER_SIZE, sizeof(char));
    }
    if(s2)
        free(s2);
    if(read_re < 0)
        return -1;
    return 0;
}

int main(int ac, char **av)
{
    char *hey;
    char *buffer = NULL;
    int len;
    //int total_read = 0;

    if(ac != 2)
        return printf("You need one argument"), 1;
    len = ft_strlen(av[1]);
    if(len == 0)
        return printf("Pleas give a hystack"), 1;
    hey = av[1];
    //total_read = use_realloc(&buffer);
    if(prep_buf(&buffer) < 0)
        return printf("allocation failed"), 1;
    filter(buffer, hey, ft_strlen(buffer));
    printf("%s", buffer);
    free(buffer);
    return 0;
}