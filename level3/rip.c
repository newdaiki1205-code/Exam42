#include <stdio.h>
#include <unistd.h>

int ft_strlen(char *s)
{
    int i = 0;

    while(s[i])
        i++;
    return i;
}

void prep_data(char *s, int *index)
{
    int i = 0;

    while(s[i])
    {
        if(s[i] == '(')
            index[i] = 1;
        else if(s[i] == ')')
            index[i] = -1;
        else
            index[i] = 0;
        i++;
    }
}

int is_sum(int *base, int len)
{
    int i = 0;
    int sum = 0;

    while(i < len)
    {
        sum += base[i];
        i++;
    }
    return sum;
}

void print_par(int *index, int len)
{
    int i = 0;

    //printf("hihi\n");
    while(i < len)
    {
        if(index[i] == 1)
            write(1, "(", 1);
        else if(index[i] == -1)
            write(1, ")", 1);
        else
            write(1, " ", 1);
        i++;
    }
    write(1, "\n", 1);
}

void check_print(int *index, int len)
{
    int i = 0;
    int j = len - 1;
    int first = 0;
    int last = 0;

    while(i < len)
    {
        if(index[i] != 0)
        {
            first = index[i];
            break;
        }
        i++;
    }
    while(j >= 0)
    {
        if(index[j] != 0)
        {
            last = index[j];
            break;
        }
        j--;
    }
    if(first == 1 && last == -1)
        print_par(index, len);
}

void rec_rip(int *index, int len, int target, int pos)
{
    while(pos < len)
    {
        if(index[pos] == target){
            index[pos] = 0;
        if(is_sum(index, len) == 0)
        {
            check_print(index, len);
            index[pos] = target;
            //return ;
        }
        else
            rec_rip(index, len, target, pos + 1);}
        if(index[pos] == 0)
            index[pos] = target;
        pos++;
    }
}

void rip(char *s, int *index, int len)
{
    int sumcheck = is_sum(index, len);
    int target;

    if(sumcheck > 0)
        target = 1;
    else if (sumcheck < 0)
        target = -1;
    else
    {
        puts(s);
        return;
    }
    rec_rip(index, len, target, 0);
}


int main(int ac, char **av)
{
    int len;

    if(ac != 2)
        return 1;
    len = ft_strlen(av[1]);
    int index[len];
    prep_data(av[1], index);
    rip(av[1], index, len);
    return 0;
}