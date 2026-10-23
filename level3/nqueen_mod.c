#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct s_data
{
    int **tab;
    int *ret;
} t_data;

void free_all(t_data *data, int i)
{
    while(i >= 0 && data->tab[i])
    {
        free(data->tab[i]);
        i--;
    }
    if(data->tab)
        free(data->tab);
    if(data->ret)
        free(data->ret);
    if(data)
        free(data);
}

int prep_data(t_data *data, int n)
{
    int i = 0;
    
    data->tab = (int **)calloc(n, sizeof(int*));
    if(!data->tab)
        return 1;
    while(i < n)
    {
        data->tab[i] = (int *)calloc(n, sizeof(int));
        if(!data->tab[i])
            return free_all(data, i - 1), 1;
        i++;
    }
    data->ret = (int *)calloc(n, sizeof(int));
    if(!data->ret)
        return free_all(data, n - 1), 1;
    return 0;
}

int tate(t_data *data, int row, int col)
{
    int check_row = row - 1;
    
    while(check_row >= 0)
    {
        if(data->tab[check_row][col] == 1)
            return 1;
        check_row--;
    }
    return 0;
}

int naname(t_data *data, int row, int col, int limit)
{
    int ul_col = col - 1;
    int ur_col = col + 1;
    int check_row = row;

    while(check_row >= 0 && ul_col >= 0)
    {
        if(data->tab[check_row][ul_col] == 1)
            return 1;
        check_row--;
        ul_col--;
    }
    check_row = row;
    while(check_row >= 0 && ur_col < limit)
    {
        if(data->tab[check_row][ur_col] == 1)
            return 1;
        check_row--;
        ur_col++;
    }
    return 0;
}

void print_ret(int *ret, int limit)
{
    int i = 0;
    char c;

    while(i < limit)
    {
        c = ret[i] + '0';
        write(1, &c, 1);
        if(i == limit - 1)
            break;
        write(1, " ", 1);
        i++;
    }
    write(1, "\n", 1);
}

void solve(t_data *data, int limit, int row, int col)
{
    while(col < limit)
    {
        data->tab[row][col] = 1;
        if(tate(data, row, col) || naname(data, row - 1, col, limit))
        {
            data->tab[row][col] = 0;
        }
        else
        {
            data->ret[row] = col;
            if(row == limit - 1)
            {
                print_ret(data->ret, limit);
                data->tab[row][col] = 0;
                //return;
            }
            else
                solve(data, limit, row + 1, 0);
            data->tab[row][col] = 0;
        }
        col++;
    }
}

int n_queen(int n)
{
    t_data *data;

    data = (t_data*)malloc(sizeof(t_data));
    if(!data)
        return 1;
    if(prep_data(data, n))
        return 1;
    solve(data, n, 0, 0);
    free_all(data, n - 1);
    return 0;
}

int input_check(char *s)
{
    int i = 0;

    while(s[i])
    {
        if(s[i] < '0' || s[i] > '9')
            return 1;
        i++;
    }
    return 0;
}

int main(int ac, char **av)
{
    int n;

    if(ac != 2)
        return write(1, "Error: Give me a number\n", 24), 1;
    if(input_check(av[1]))
        return write(1, "Error: Argument should be a number\n", 35), 1;
    n = atoi(av[1]);
    if(n < 2)
        return 1;
    if(n_queen(n))
        return write(1, "Error: Allocation failed\n", 25);
    return 0; 
}