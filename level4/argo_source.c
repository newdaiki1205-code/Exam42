
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

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
int	peek(FILE *stream);
void	unexpected(FILE *stream);
int	accept(FILE *stream, char c);
int	expect(FILE *stream, char c);
void	free_json(json j);
void	serialize(json j);

int parse_map(json *dst, FILE *stream);
int parse_int(json *dst, FILE *stream);
int parser(json *dst, FILE *stream);
int argo(json *dst, FILE *stream);
char *get_str(FILE *stream);

int	peek(FILE *stream) //return current char in the stream (not consume stream)
{
	int	c = getc(stream);
	ungetc(c, stream);
	return c;
}

void	unexpected(FILE *stream) //error message when char is unexpected
{
	if (peek(stream) != EOF)
		printf("unexpected token '%c'\n", peek(stream));
	else
		printf("unexpected end of input\n");
}

int	accept(FILE *stream, char c) //check current char with the expected char and move to next char
{
	if (peek(stream) == c)
	{
		(void)getc(stream);
		return 1;
	}
	return 0;
}

int	expect(FILE *stream, char c) //move to next char or error message
{
	if (accept(stream, c))
		return 1;
	unexpected(stream);
	return 0;
}

void	free_json(json j) //free ast
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

void	serialize(json j) //print function
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


/*You should code from here - parser in general, parser for three cases, argo*/

int	parse_int(json *dst, FILE *stream) //save data in n -> save data in json file
{
	int n = 0;

	fscanf(stream, "%d", &n);
	dst->type = INTEGER;
	dst->integer = n;
	return (1);
}

char *get_str(FILE *stream) 
{
	char *res = calloc(4096, sizeof(char)); //prepare buf
	int i = 0;
	char c = getc(stream); //now (c = ") and stream points to the next char after double quote 

	while (1) 
	{
		c = getc(stream); //now (c = the start char of str)
		
		if (c == '"') //break point = the end of the str should be closed double quoation
			break ;
		if (c == EOF) //input error
		{
			unexpected(stream);
			return NULL;
		}
		if (c == '\\') //suject requires us to handle only escaping backslash and quotation
			c = getc(stream);
		res[i++] = c;
	}
	return (res);
}

int parse_map(json *dst, FILE *stream)
{
	dst->type = MAP;
	dst->map.size = 0; //initialize
	dst->map.data = NULL; //initialize
	char c = getc(stream); //now (c = {)

	if (peek(stream) == '}')
		return 1;

	while (1)
	{
		c = peek(stream);
		if (c != '"') //map should be {"...":...}
		{
			unexpected(stream);
			return -1;
		}
		dst->map.data = realloc(dst->map.data, (dst->map.size + 1) * sizeof(pair)); //make array of structs
		pair *current = &dst->map.data[dst->map.size]; //points to new pos in array of structs
		current->key = get_str(stream); //assign key in array of struct (key)
		if (current->key == NULL)
			return -1;
		dst->map.size++;
		if (expect(stream, ':') == 0) //check syntax {"Key":Value} now stream points to the head of value
			return -1;
		if (argo(&current->value, stream) == -1) //assign value
			return -1;
		c = peek(stream); //check current char
		if (c == '}') //it is closed
		{
			accept(stream ,c);
			break ;
		}
		if (c == ',') //it has still key and value
			accept(stream, c);
		else
		{
			unexpected(stream); //error
			return -1;
		}
	}
	return 1;
}

/*
Parser handles three cases
1. int
2. str: "string"
3. map: {"Key":"Value (int or str or map)"}
*/

int parser(json *dst, FILE *stream)
{
	int c = peek(stream);

	if (c == EOF) //protect
	{
		unexpected(stream);
		return -1;
	}
	if (isdigit(c)) // case int
		return (parse_int(dst, stream));
	else if (c == '"') //case str: always start with \"
	{
		dst->type = STRING;
		dst->string = get_str(stream);
		if (dst->string == NULL)
			return (-1);
		return (1);
	}
	else if (c == '{') //case map
		return (parse_map(dst, stream));
	else
	{
		unexpected(stream);
		return -1;
	}
	return (1);
}

int argo(json *dst, FILE *stream)
{
	if (parser(dst, stream) == -1)
		return -1;
	return 1;
}

int	main(int argc, char **argv)
{
	if (argc != 2)
		return 1;
	char *filename = argv[1];
	FILE *stream = fopen(filename, "r");
	if (!stream)
		return (1);
	json	file;
	if (argo (&file, stream) != 1)
	{
		free_json(file);
		return 1;
	}
	serialize(file);
	printf("\n");
}
