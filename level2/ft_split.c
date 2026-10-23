#include <stdlib.h>
#include <stdio.h>

int ft_strlen(char *str)
{
    int i = 0;

    while(str[i])
        i++;
    return i;
}


int del_count(char *str)
{
    int i = 0;
    int del = 0;

    while(!(str[i] >= 33 && str[i] < 127))
        i++;
    while(str[i++])
    {
        if (str[i] == '\n' || str[i] == '\t' || str[i] == ' ')
        {
            del++;
            while(str[i] && !(str[i] >= 33 && str[i] < 127))
                i++;
        }
    }
    if(!(str[ft_strlen(str) - 1] >= 33 && str[ft_strlen(str) - 1] < 127))
        del--;
    return (del + 1);
}

char *make_unit(char *str)
{
    int size = 0;
    int i = 0;
    char *new;
    
    while (str[size] && (str[size] >= 33 && str[size] < 127))
        size++;
    new = (char *)malloc(sizeof(char) * (size + 1));
    if(!new)
        return (NULL);
    while(i < size)
    {
        new[i] = *str;
        str++;
        i++;
    }
    new[size] = '\0';
    return (new);
}

void free_str(char **str, int i)
{
    i--;
    while(i >= 0)
    {
        free(str[i]);
        i--;
    }
    free(str);
}


char    **ft_split(char *str)
{
    if(!str)
        return NULL;
    
    char **new;
    int size = del_count(str);
    int i = 0;

    new = (char **)malloc(sizeof(char *) * (size + 1));
    if(!new)
        return (NULL);
    while(i < size)
    {
        while(*str && !(*str >= 33 && *str < 127))
            str++;
        new[i] = make_unit(str);
        if(!new)
            return (free_str(new, i), NULL);
        while(*str && (*str >= 33 && *str < 127))
            str++;
        i++;
    }
    new[i] = NULL;
    return (new);
}

int main(void)
{
    char **test;
    char src[] = "";

    test = ft_split(src);
    int i = 0;
    while(test && test[i])
    {
        printf("%s\n", test[i]);
        i++;
    }
    i = 0;
    while(test && test[i])
    {
        free(test[i]);
        i++;
    }
    free(test);
    return 0;
}