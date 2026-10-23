#include <unistd.h>
#include <stdio.h>

int get_adress(const char c)
{
    char *base_low = "0123456789abcdef";
    char *base_up = "0123456789ABCDEF";
    int i = 0;

    while(1)
    {
        if(c == base_low[i])
            break;
        if(c == base_up[i])
            break ;
        i++;
    }
    return i;
}


int	ft_atoi_base(const char *str, int str_base)
{
    int power = 0;
    int result = 0;
    int base_adress;
    int i = 0;
    int save;
    int count;

    while(str[power])
        power++;
    while(power > 0)
    {
        count = 0;
        save = 1;
        base_adress = get_adress(str[i]);
        while(count < power - 1)
        {
            save *= str_base;
            count++;
        }
        save *= base_adress;
        result +=save;
        power--;
        i++;
    }
    return result;
}

int main(void)
{
    char *str = "0";
    int str_base = 16;

    int dec = ft_atoi_base(str, str_base);
    printf("%d", dec);
}