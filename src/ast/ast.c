#include "ast.h"

#include <err.h>
#include <stdlib.h>
#include <stdio.h>

#include "../utils/alloc.h"

struct ast *ast_new(enum ast_type type)
{
    struct ast *new = zalloc(sizeof(struct ast));
    new->type = type;
    return new;
}

void ast_free(struct ast *ast)
{
    if (ast == NULL)
        return;
    /*if the ast has allocated children, free them recursively */
    if (ast->children)
    {
        int child = 0;
        while (ast->children[child])
        {
            ast_free(ast->children[child]);
            child++;
        }
        free(ast->children);
    }

    /*if the ast has allocated data, free it*/
    if (ast->data)
    {
        size_t i = 0;
        while (ast->data[i])
        {
            free(ast->data[i]);
            i++;
        }
        free(ast->data);
    }

    free(ast);
}

void print_ast(struct ast *ast)
{
    if (ast == NULL)
        return;

    if (ast->type == AST_SIMPLE_COMM)
    {
        if (ast->children)
        {
            size_t i = 0;
            while (ast->children[i])
            {
                if (!ast->children[i + 1])
                {
                    print_ast(ast->children[i]);
                    i++;
                }
                else
                {
                    print_ast(ast->children[i]);
                    putchar(32);
                    i++;
                }
            }
        }
        else
        {
            printf("null");
        }
    }
    else if (ast->type == AST_WORD)
    {
        if (ast->data)
        {
            printf("[%s]", ast->data[0]);
        }
        else
        {
            printf("[null]");
        }
    }
    else if (ast->type == AST_LIST)
    {
        if (ast->children)
        {
            size_t i = 0;
            while (ast->children[i])
            {
                print_ast(ast->children[i]);
                putchar(';');
                i++;
            }
        }
    }
    else if (ast->type == AST_IF)
    {
        printf("if ");
        putchar('(');
        print_ast(ast->children[0]);
        putchar(')');

        printf(" then (");
        print_ast(ast->children[1]);
        putchar(')');

        if (ast->children[2])
        {    
            printf(" else (");
            print_ast(ast->children[2]);
            putchar(')');
        }
        printf(" fi");
     }
    else if (ast->type == AST_PIPELINE)
    {
        if (ast->children)
        {
            size_t i = 0;
            while (ast->children[i])
            {
                if (!ast->children[i + 1])
                {
                    print_ast(ast->children[i]);
                    i++;
                }
                else
                {
                    print_ast(ast->children[i]);
                    printf(" | ");
                    i++;
                }
            }
        }
        else
        {
            printf("This pipeline has no children table!");
        }
    }
    else if (ast->type == AST_NEGATION)
    {
        if (ast->children)
        {
            putchar('!');
            putchar(32);
            print_ast(ast->children[0]);
        }
    }
    else
    {
        printf("don't know this one yet");
    }
}
