#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h> // change this to <stdlib.h>


typedef struct	json {
	enum {
		MAP,
		INTEGER,
		STRING
	} type;
	union {
		struct {
			struct pair	*data;
			size_t		size;
		} map;
		int	integer;
		char	*string;
	};
}	json;

typedef struct	pair {
	char	*key;
	json	value;
}	pair;

void	free_json(json j);
int	argo(json *dst, FILE *stream);

int	peek(FILE *stream)
{
	int	c = getc(stream);
	ungetc(c, stream);
	return c;
}

void	unexpected(FILE *stream)
{
	if (peek(stream) != EOF)
		printf("unexpected token '%c'\n", peek(stream));
	else
		printf("unexpected end of input\n");
}

int	accept(FILE *stream, char c)
{
	if (peek(stream) == c)
	{
		(void)getc(stream);
		return 1;
	}
	return 0;
}

int	expect(FILE *stream, char c)
{
	if (accept(stream, c))
		return 1;
	unexpected(stream);
	return 0;
}

void	free_json(json j)
{
	switch (j.type)
	{
		case MAP:
			for (size_t i = 0; i < j.map.size; i++)
			{
				free(j.map.data[i].key);
				free_json(j.map.data[i].value);
			}
			free(j.map.data);
			break ;
		case STRING:
			free(j.string);
			break ;
		default:
			break ;
	}
}

void	serialize(json j)
{
	switch (j.type)
	{
		case INTEGER:
			printf("%d", j.integer);
			break ;
		case STRING:
			putchar('"');
			for (int i = 0; j.string[i]; i++)
			{
				if (j.string[i] == '\\' || j.string[i] == '"')
					putchar('\\');
				putchar(j.string[i]);
			}
			putchar('"');
			break ;
		case MAP:
			putchar('{');
			for (size_t i = 0; i < j.map.size; i++)
			{
				if (i != 0)
					putchar(',');
				serialize((json){.type = STRING, .string = j.map.data[i].key});
				putchar(':');
				serialize(j.map.data[i].value);
			}
			putchar('}');
			break ;
	}
}

int parse_int(json *dst, FILE *stream)
{
    int n;

    fscanf(stream, "%d", &n);
    dst->type = INTEGER;
    dst->integer = n;
    return 1;
}

char *get_str(FILE *stream)
{
    char *str = (char *)calloc(4096, sizeof(char));
    char c = getc(stream); //now "
    int i = 0;

    if(!str)
        return NULL;
    while(1)
    {
        c = getc(stream);
        if(c == '"')
            break;
        if(c == EOF)
        {
            unexpected(stream);
            return NULL;
        }
        if(c == '\\')
            c = getc(stream);
        str[i] = c;
        i++;
    }
    return str;
}

/*
Syntax Check
    1. Key should start with double quote
    2. Between Key and Value should be colon
    3.1 if it close, it should be } 
    3.2 if it proceeds, it should be conma


*/

int parser_map(json *dst, FILE *stream)
{
    dst->type = MAP;
    dst->map.data = NULL;
    dst->map.size = 0;
    char c = getc(stream); //now c = { stream = "
    pair *tmp;

    while(1)
    {
        /*Start Check*/
        if(peek(stream) != '"')
        {
            unexpected(stream);
            return -1;
        }

        /*Prep Copy*/
        dst->map.data = realloc(dst->map.data, (dst->map.size + 1) * sizeof(pair));
        if(!dst->map.data)
            return -1;
        tmp = &dst->map.data[dst->map.size]; //to make it easy to refer to key and value (too long!!)
        
        /*Copy*/
        tmp->key = get_str(stream);
        if(!tmp->key)
            return -1;
        if(!expect(stream, ':'))
            return -1;
        if(argo(&tmp->value, stream) == -1)
            return -1;

        dst->map.size++; //if copy is done, size should be incremented (for print function)

        /*End Check*/
        c = peek(stream);
        if(c == '}')
        {
            accept(stream, c); //need for nest case
            break;
        }
        else if(c == ',')
        {
            accept(stream, c);
        }
        else
        {
            unexpected(stream);
            return -1;
        }
    }
    return 1;
}

int	parser(json *dst, FILE *stream)
{
    char c = peek(stream);
    
    if(!dst || !stream)
        return -1;
    if(isdigit(c))
        return (parse_int(dst, stream));
    else if(c == '"')
    {
        dst->type = STRING;
        dst->string = get_str(stream);
        if(!dst->string)
            return -1;
        return 1;
    }
    else if(c == '{')
        return (parser_map(dst, stream));
    else
    {
        unexpected(stream);
        return -1;
    }   
}


int	argo(json *dst, FILE *stream)
{
    if(parser(dst, stream) == -1)
        return -1;
    return 1;
}


int	main(int argc, char **argv)
{
	if (argc != 2)
		return 1;
	char *filename = argv[1];
	FILE *stream = fopen(filename, "r");
	json	file;
	if (argo (&file, stream) != 1)
	{
		free_json(file);
		return 1;
	}
	serialize(file);
	printf("\n");
}
