#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct s_data
{
	char	**tab;
	char	*save;
	int		count;
	int		len;
	int		size;
}			t_data;

int	ft_strlen(char *s)
{
	int	i;

	i = 0;
	if (!s)
		return (-1);
	while (s[i])
		i++;
	return (i);
}

void	ft_strcpy(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s2[i])
	{
		s1[i] = s2[i];
		i++;
	}
}

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i])
	{
		if (s1[i] != s2[i])
			return (s1[i] - s2[i]);
		i++;
	}
	return (0);
}

void	swap(char *a, char *b)
{
	char	temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

void	permutation(t_data *data, char *s, int pos, int limit)
{
	int	target;

	target = pos;
	if (pos == limit)
	{
		ft_strcpy(data->tab[data->count], s);
		data->count++;
		return ;
	}
	while (target < limit)
	{
		swap(s + pos, s + target);
		permutation(data, s, pos + 1, limit);
		swap(s + pos, s + target);
		target++;
	}
}

int	count_size(int len)
{
	int	re;
	int	i;

	re = 1;
	i = 0;
	while (i < len)
	{
		re = re * (len - i);
		i++;
	}
	return (re);
}

void	free_all(t_data *data, int i)
{
	if (data->save)
		free(data->save);
	while (i >= 0 && data->tab[i])
	{
		free(data->tab[i]);
		i--;
	}
	if (data->tab)
		free(data->tab);
	if (data)
		free(data);
}

int	make_table(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->size)
	{
		data->tab[i] = (char *)calloc(data->len + 1, sizeof(char));
		if (!data->tab[i])
			return (free_all(data, i - 1), 0);
		i++;
	}
	return (1);
}

int	prep_data(t_data *data)
{
	data->size = count_size(data->len);
	data->tab = (char **)calloc(data->size + 1, sizeof(char *));
	if (!data->tab)
		return (free(data), 0);
	if (!make_table(data))
		return (0);
	data->save = (char *)calloc(data->len + 1, sizeof(char));
	if (!data->save)
		return (free_all(data, data->size - 1), 0);
	return (1);
}

void	sort_order(t_data *data)
{
	int	i;
	int	j;

	i = 0;
	while (i < data->size)
	{
		j = 0;
		while (j < data->size)
		{
			if (ft_strcmp(data->tab[i], data->tab[j]) < 0)
			{
				ft_strcpy(data->save, data->tab[i]);
				ft_strcpy(data->tab[i], data->tab[j]);
				ft_strcpy(data->tab[j], data->save);
			}
			j++;
		}
		i++;
	}
	return ;
}

int	main(int ac, char **av)
{
	t_data	*data;
	int		i;

	if (ac != 2)
		return (1);
	data = (t_data *)malloc(sizeof(t_data));
	if (!data)
		return (1);
	data->len = ft_strlen(av[1]);
	if (!prep_data(data))
		return (1);
	data->count = 0;
	permutation(data, av[1], 0, data->len);
	sort_order(data);
	i = 0;
	while (i < data->size)
	{
		printf("tab[%i] %s\n", i, data->tab[i]);
		i++;
	}
	free_all(data, data->size - 1);
	return (0);
}