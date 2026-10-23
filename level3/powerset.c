#include <stdio.h>
#include <stdlib.h>

typedef struct s_data
{
	int	target;
	int	*base;
    int *index;
    int size_array;
    int endpoint;
}		t_data;

void printout(t_data *data, int size)
{
    int i = 0;
    int count = 0;

    while(i <= data->size_array)
    {
        if(data->index[i])
        {
            printf("%d", data->base[i]);
            count++;
            if(count != size)
                printf(" ");
        }
        i++;
    }
    printf("\n");
}

void sum_is(t_data *data)
{
    int i = 0;
    int sum = 0;
    int count = 0;

    while(i < data->size_array)
    {
        if(data->index[i])
        {
            sum = sum + data->base[i];
            count++;
        }  
        i++;
    }
    if(sum == data->target)
        printout(data, count);
}

void findset(t_data *data, int checkpoint)
{
    while(checkpoint < data->endpoint)
    {
        data->index[checkpoint] = 1;
        sum_is(data);
        findset(data, checkpoint + 1);
        data->index[checkpoint] = 0;
        checkpoint++;
    }
}

int powerset(t_data *data, int subset_size)
{
    int i = 0;

    while(i < data->size_array)
    {
        data->index[subset_size - 1] = 1;
        sum_is(data);
        data->endpoint = subset_size - 1;
        findset(data, 0);
        data->index[subset_size - 1] = 0;
        i++;
        subset_size--;
    }
    return 0;
}

void free_all(t_data *data)
{
    if(data->index)
        free(data->index);
    if(data->base)
        free(data->base);
    if(data)
        free(data);
}

void bubble_sort(int *arr, int max)
{
    int i = 0;
    int j;
    int tmp;

    while(i < max)
    {
        j = i;
        while(j < max)
        {
            if(arr[i] > arr[j])
            {
                tmp = arr[i];
                arr[i] = arr[j];
                arr[j] = tmp;
            }
            j++;
        }
        i++;
    }
}

int prep_data(t_data *data, int ac, char **av)
{
    int num;
    int i = 2;
    int pos = 0;
    
    data->target = atoi(av[1]);
    data->size_array = ac - 2;
    data->base = (int*)malloc(data->size_array * sizeof(int));
    if(!data->base)
        return free_all(data), 0;
    data->index = (int*)calloc(data->size_array, sizeof(int));
    if(!data->index)
        return free_all(data), 0;
    while(pos < data->size_array)
    {
        num = atoi(av[i]);
        data->base[pos] = num;
        i++;
        pos++;
    }
    bubble_sort(data->base, data->size_array);
    return 1;
}

int	main(int ac, char **av)
{
	t_data *data;

	if (ac < 3)
		return (0);
	data = (t_data *)malloc(sizeof(t_data));
	if (!data)
		return (1);
    if(!prep_data(data, ac, av))
        return 1;
    printf("target %d\n", data->target);
    if(powerset(data, data->size_array))
        return 1;
    free_all(data);
    return 0;
}