#include <stdio.h>
#include <stdlib.h> // change this to <stdlib.h>
#include <ctype.h>
#include <unistd.h>

typedef enum {
        ADD,
        MULTI,
        VAL
    }   type;

typedef struct node {
    type type;
    int val;
    struct node *l;
    struct node *r;
}   node;


node    *new_node(node n)
{
    node *ret = calloc(1, sizeof(n));
    if (!ret)
        return (NULL);
    *ret = n;
    return (ret);
}

void    destroy_tree(node *n)
{
    if (!n)
        return ;
    if (n->type != VAL)
    {
        destroy_tree(n->l);
        destroy_tree(n->r);
    }
    free(n);
}

void    unexpected(char c)
{
    if (c)
        printf("Unexpected token '%c'\n", c);
    else
        printf("Unexpected end of input\n");
}

int accept(char **s, char c)
{
    (void)c;
    if (**s)
    {
        (*s)++;
        return (1);
    }
    return (0);
}

int expect(char **s, char c)
{
    if (accept(s, c))
        return (1);
    unexpected(**s);
    return (0);
}

int eval_tree(node *tree)
{
    if(!tree)
        exit(1);
    
    switch (tree->type)
    {
        case ADD:
            return (eval_tree(tree->l) + eval_tree(tree->r));
        case MULTI:
            return (eval_tree(tree->l) * eval_tree(tree->r));
        case VAL:
            return (tree->val);
    }
}


node *make_node(type type, node *lhs, node *rhs)
{
    node *new = (node *)calloc(1, sizeof(node));

    if(!new || !rhs)
    {
        destroy_tree(lhs);
        destroy_tree(rhs);
        return NULL;
    }
    new->type = type;
    new->l = lhs;
    new->r = rhs;
    return new;
}

node *make_node_num(char val)
{
    node *new = (node *)calloc(1, sizeof(node));

    if(!new)
        return NULL;
    new->type = VAL;
    new->val = val - '0';
    return new;
}

node *parse_expr(char **input, int flag);

node *primary(char **input)
{
    node *new;

    if(*(*input) && isdigit(*(*input)))
    {
        new = make_node_num(*(*input));
        if(!new)
            return NULL;
        accept(input, 'c');
    }
    else if(*(*input) && *(*input) == '(')
    {
        accept(input, 'c');
        new = parse_expr(input, 1);
        if(!new)
            return NULL;
        if(!expect(input, ')'))
            return destroy_tree(new), NULL;
    }
    else
    {
        unexpected(**input);
        return NULL;
    }
    if(*(*input) && isdigit(*(*input)))
    {
        destroy_tree(new);
        unexpected(**input);
        return NULL;
    }
    return new;
}

node *mul(char **input)
{
    node *ret = primary(input);
    if(!ret)
        return NULL;
    
    //if(*(*input) && *(*input) == '*')
    while(*(*input) && *(*input) == '*')
    {
        accept(input, '*');
        ret = make_node(MULTI, ret, primary(input));
        if(!ret)
            return NULL;
    }
    return ret;
}


node *parse_expr(char **input, int flag) //need flag
{
    node *ret = mul(input);
    if(!ret)
        return NULL;
    
    //if(*(*input) && *(*input) == '+')
    while(*(*input) && *(*input) == '+')
    {
        accept(input, '+');
        ret = make_node(ADD, ret, mul(input));
        if(!ret)
            return NULL;
    }
    if(**input && !flag)
    {
        unexpected(**input);
        destroy_tree(ret);
        return NULL;
    }
    return ret;
}


int main(int argc, char **argv)
{
    if (argc != 2)
        return (1);
    node *tree = parse_expr(&argv[1], 0);
    if (!tree)
        return (1);
    printf("%d\n", eval_tree(tree));
    destroy_tree(tree);
}
