#include <stdarg.h>
#include <stdio.h>
#include <ctype.h>

int match_space(FILE *f)
{
    int was;

    was = getc(f);
    if(was == EOF && ferror(f))
        return -1;
    while(was != EOF)
    {
        if(!isspace(was))
        {
            ungetc(was, f);
            break;
        }
        was = fgetc(f);
    }
    if(ferror(f))
        return -1;
    return (0);
}

int match_char(FILE *f, char c)
{
    int ch = fgetc(f);

    if(ch == EOF && ferror(f))
        return -1;
    else if (ch == EOF)
        return 0;
    if(ch == c)
        return 1;
    else
        ungetc(ch, f);
    return (0);
}

int scan_char(FILE *f, va_list *ap)
{
    char *pc = va_arg(*ap, char *);
    int ch = fgetc(f);

    if(ch == -1)
        return -1;
    *pc = ch;
    return (1);
}

int scan_int(FILE *f, va_list *ap)
{
    int *pi = va_arg(*ap, int*);
    int num = fgetc(f);
    long long sum = 0;
    int sign = 1;

    if(num == '-' || num == '+')
    {
        if(num == '-')
            sign = -1;
        num = fgetc(f);
        if(num == EOF && ferror(f))
            return -1;
    }
    while('0' <= num && num <= '9')
    {
        if(sum > 2147483647 || sum * -1 < -2147483648)
            return printf("Overflow: Input is too big"), -1;
        sum = sum * 10 + (num - '0');
        num = fgetc(f);
        if(num == EOF)
            break;
    }
    if(ferror(f))
        return -1;
    *pi = sum * sign;
    return (1);
}

int scan_string(FILE *f, va_list *ap)
{
    char *ps = va_arg(*ap, char *);
    int i = 0;
    int was = fgetc(f);

    while(was != EOF && !isspace(was))
    {
        ps[i] = was;
        i++;
        was = fgetc(f);
    }
    ps[i] = '\0';
    if(ferror(f))
        return -1;
    return (0);
}


int	match_conv(FILE *f, const char **format, va_list *ap)
{
	switch (**format)
	{
		case 'c':
			return scan_char(f, ap);
		case 'd':
			match_space(f);
			return scan_int(f, ap);
		case 's':
			match_space(f);
			return scan_string(f, ap);
		case EOF:
			return -1;
		default:
			return -1;
	}
}

int ft_vfscanf(FILE *f, const char *format, va_list ap)
{
	int nconv = 0;

    int c = fgetc(f);
	if (c == EOF)
		return EOF;
	ungetc(c, f);

	while (*format)
	{
		if (*format == '%')
		{
			format++;
			if (match_conv(f, &format, &ap) != 1)
				break;
			else
				nconv++;
		}
		else if (isspace(*format))
		{
			if (match_space(f) == -1)
				break;
		}
		else if (match_char(f, *format) != 1)
			break;
		format++;
	}
	
	if (ferror(f))
		return EOF;
	return nconv;
}


int ft_scanf(const char *format, ...)
{
	va_list ap;

    va_start(ap, format);
	int ret = ft_vfscanf(stdin, format, ap);
	if(ret == EOF)
        return -1; 
	return ret;
}

int main(void)
{
    char c;
    int i;
    char s[100];

    ft_scanf("%c %d %s",&c, &i, s);
    printf("%c %d %s",c, i, s);
    // int ret = ft_scanf("%c %d", &c, &i);
    // printf("%d\n", ret);
    // printf("%d",i);
    return 0;
}