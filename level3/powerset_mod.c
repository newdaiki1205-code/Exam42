#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

typedef struct s_data
{
    int *base;
    int *index;
    int size;
    int target;
} t_data;

void free_all(t_data *data)
{
    if(data->index)
        free(data->index);
    if(data->base)
        free(data->base);
    if(data)
        free(data);    
}

void sort_order(t_data *data)
{
    int i = 0;
    int j;
    int tmp;

    while(i < data->size)
    {
        j = i;
        while(j < data->size)
        {
            if(data->base[i] > data->base[j])
            {
                tmp = data->base[i];
                data->base[i] = data->base[j];
                data->base[j] = tmp;
            }
            j++;
        }
        i++;
    }
}

long long ft_atol(char *s)
{
    long long num = 0;
    int sign = 1;
    int i = 0;

    if(s[i] == '-' || s[i] == '+')
    {
        if(s[i] == '-')
            sign = -1;
        i++;
    }
    while(s[i])
    {
        num = num * 10 + (s[i] - '0');
        i++;
    }
    num = num * sign;
    return num;
}

int prep_data(t_data *data, int ac, char **av)
{
    int i = 1;
    int j;
    int pos = 0;
    long long num;

    data->base = (int*)calloc(ac - 2, sizeof(int));
    if(!data->base)
        return free_all(data), 1;
    while(i < ac)
    {
        j = 0;
        while(av[i][j])
        {
            if(av[i][j] != '-' && av[i][j] != '+' && (av[i][j] < '0' || av[i][j] > '9'))
                return free_all(data), 1;
            j++;
        }
        num = ft_atol(av[i]);
        if(num < -2147483648 || 2147483647 < num)
            return free_all(data), 1;
        if(i == 1)
            data->target = num;
        else
        {
            data->base[pos] = num;
            pos++;
        }
        i++;
    }
    data->size = ac - 2;
    //sort_order(data);
    data->index = (int *)calloc(ac - 2, sizeof(int));
    if(!data->index)
        return free_all(data), 1;
    return 0;
}

int keisan(t_data* data, int fix_pos)
{
    int total = 0;
    int i = 0;

    while(i <= fix_pos)
    {
        if(data->index[i] == 1)
            total += data->base[i];
        i++;
    }
    return total;
}

void print_set(t_data *data, int fix_pos)
{
    int i = 0;

    while(i <= fix_pos)
    {
        if(data->index[i] == 1)
        {
            printf("%i", data->base[i]);
            if(i == fix_pos)
                printf("\n");
            else
                printf(" ");
        } 
        i++;
    }
}

void find_set(t_data *data, int fix_pos, int search)
{
    int total;

    while(search < fix_pos)
    {
        data->index[search] = 1;
        total = keisan(data, fix_pos);
        if(total == data->target)
        {
            print_set(data, fix_pos);
            data->index[search] = 0;
            //return;
        }
        else
        {
            find_set(data, fix_pos, search + 1);
            data->index[search] = 0;
        }
        search++;
    }
}

void powerset(t_data *data, int fix_pos)
{
    while(fix_pos >= 0)
    {
        if(data->base[fix_pos] == data->target)
            printf("%d\n", data->target);
        data->index[fix_pos] = 1;
        find_set(data, fix_pos, 0);
        data->index[fix_pos] = 0;
        fix_pos--;
    }
}

int main(int ac, char **av)
{
    t_data *data;

    if(ac < 2)
        return 1;
    data = (t_data*)malloc(sizeof(t_data));
    if(!data)
        return 1;
    if(prep_data(data, ac, av))
        return 1;
    powerset(data, data->size - 1);
    free_all(data);
    return 0;
}