#include <stdlib.h>
#include <stdio.h>

typedef struct s_data
{
    char *source;
    char *result;
    int *index;
    int len;
} t_data;

int ft_isalpha(char *s)
{
    int i = 0;

    while(s[i])
    {
        if(!(s[i] >= 'A' && s[i] <= 'Z') && !((s[i] >= 'a' && s[i] <= 'z')))
            return 1;
        i++;
    }
    return 0;
}

int inputcheck(int ac, char *s)
{
    if(ac != 2)
        return 1;
    if(ft_isalpha(s))
        return 1;
    return 0;
}

char *sort_oder(char *s)
{
    int i = 0;
    int j;
    char tmp;

    while(s[i])
    {
        j = i;
        while(s[j])
        {
            if(s[i] > s[j])
            {
                tmp = s[i];
                s[i] = s[j];
                s[j] = tmp;
            }
            j++;
        }
        i++;
    }
    return s;
}

int ft_strlen(char *s)
{
    int i = 0;

    while(s[i])
        i++;
    return i;
}

void free_all(t_data* data)
{
    if(data->index)
        free(data->index);
    if(data->result)
        free(data->result);
    if(data)
        free(data);
}

int prep_data(t_data *data, char *s)
{
    data->len = ft_strlen(s);
    data->source = sort_oder(s);
    data->result = (char *)calloc(data->len + 1, sizeof(char));
    if(!data->result)
        return free_all(data), 1;
    data->index = (int *)calloc(data->len, sizeof(int));
    if(!data->index)
        return free_all(data), 1;
    return 0;
}

void permutation(t_data *data, int pos)
{
    int i = 0;
    
    if(pos == data->len)
    {
        data->result[pos] = '\0';
        puts(data->result);
        return ;
    }
    while(i < data->len)
    {
        if(!data->index[i])
        {
            data->index[i] = 1;
            data->result[pos] = data->source[i];
            permutation(data, pos + 1);
            data->index[i] = 0;
        }
        i++;
    }
}

int main(int ac, char **av)
{
    t_data *data;

    if(inputcheck(ac, av[1]))
        return 1;
    data = (t_data*)malloc(sizeof(t_data));
    if(!data)
        return 1;
    if(prep_data(data, av[1]))
        return 1;
    permutation(data, 0);
    free_all(data);
    return 0;
}