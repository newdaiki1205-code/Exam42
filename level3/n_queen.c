#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct  s_data
{
    int **table;
    int *re_array;
    int pos_row;
    int pos_col;
}   t_data;

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

void free_reverse(int **table, int i)
{
    while(i >= 0)
    {
        if(table[i])
            free(table[i]);
        i--;
    }
    if(table)
        free(table);
}

int prep_table(t_data *data, int n)
{
    int i = 0;
    
    data->table = (int **)calloc(n, sizeof(int*));
    if(!data->table)
        return 0;
    while(i < n)
    {
        data->table[i] = (int *)calloc(n, sizeof(int));
        if(!data->table[i])
            return free_reverse(data->table, i - 1), 0;
        i++;
    }
    data->re_array = (int *)calloc(n, sizeof(int));
    if(!data->re_array)
        return free_reverse(data->table, n), 0;
    return 1;
}

void print_array(int *array, int limit)
{
    int i = 0;
    char num;

    while(i < limit)
    {
        num = array[i] + '0';
        write(1, &num, 1);
        if(i != limit - 1)
            write(1, " ", 1);
        i++;
    }
    write(1, "\n", 1);
}

int tate(t_data *data, int limit)
{
    int up = data->pos_row - 1;
    int down = data->pos_row + 1;
    
    while(up >= 0)
    {
        if(data->table[up][data->pos_col] == 1)
            return 0;
        up--;
    }
    while(down < limit)
    {
        if(data->table[down][data->pos_col] == 1)
            return 0;
        down++;
    }
    return limit;
}

int naname_slash(t_data *data, int limit)
{
    int up_row = data->pos_row - 1;
    int up_col = data->pos_col + 1;
    int down_row = data->pos_row + 1;
    int down_col = data->pos_col - 1;

    while(up_row >= 0 && up_col < limit)
    {
        if(data->table[up_row][up_col] == 1)
            return 0;
        up_row--;
        up_col++;
    }
    while(down_row < limit && down_col >= 0)
    {
        if(data->table[down_row][down_col] == 1)
            return 0;
        down_row++;
        down_col--;
    }
    return 1;
}

int naname_backslash(t_data *data, int limit)
{
    int up_row = data->pos_row - 1;
    int up_col = data->pos_col - 1;
    int down_row = data->pos_row + 1;
    int down_col = data->pos_col + 1;

    while(up_row >= 0 && up_col >= 0)
    {
        if(data->table[up_row][up_col] == 1)
            return 0;
        up_row--;
        up_col--;
    }
    while(down_row < limit && down_col < limit)
    {
        if(data->table[down_row][down_col] == 1)
            return 0;
        down_row++;
        down_col++;
    }
    return limit;
}

void solve(t_data *data, int limit, int current_row, int current_col)
{ 
    if(current_row == limit)
    {
        print_array(data->re_array, limit);
        return;
    }
    while(current_col < limit)
    {
        data->pos_row = current_row;
        data->pos_col = current_col;
        if(tate(data, limit) && naname_slash(data, limit) && naname_backslash(data, limit))
        {
            data->table[current_row][current_col] = 1;
            data->re_array[current_row] = current_col;
            solve(data, limit, current_row + 1, 0);
        }
        if(data->table[current_row][current_col] == 1)
            data->table[current_row][current_col] = 0;
        current_col++;    
    }
}

int n_queen(int n)
{
    t_data *data;
    int col_first = 0;

    data = (t_data *)malloc(sizeof(t_data));

    while(col_first < n)
    {
        if(!prep_table(data, n))
            return 1;
        data->table[0][col_first] = 1;
        data->re_array[0] = col_first;
        solve(data, n, 1, 0);
        free_reverse(data->table, n);
        free(data->re_array);
        col_first++;
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
    if(n_queen(n))
        return write(1, "Error: Allocation failed\n", 25);
    return 0; 
}