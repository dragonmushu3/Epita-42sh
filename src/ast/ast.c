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
        size_t i = 0;
        if (ast->data)
        {
            while (ast->data[i])
            {
                if (!ast->data[i+1])
                {
                    printf("[%s]", ast->data[i]);
                    i++;
                }
                else
                {
                    printf("[%s] ", ast->data[i]);
                    i++;
                }
            }
        }
        else
        {
            printf("[null]");
        }
    }
    else if (ast->type == AST_COMM)
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
//    else if (ast->type == AST_IF)
//    {
//        printf("if ");
//        putchar(')');
//        print_ast(ast->left);
//        putchar(')');
//
//        printf("then (");
//        print_ast(ast->right);
//        putchar(')');
//    }
//    else if (ast->type == AST_PIPE)
//    {
//        print_ast(ast->left);
//        printf(" | ");
//        print_ast(ast->right);
//    }
    else
    {
        printf("don't know this one yet");
    }
}
