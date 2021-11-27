#include "ast.h"

#include <err.h>
#include <stdlib.h>

#include "alloc.h"

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

    ast_free(ast->left);
    ast->left = NULL;

    ast_free(ast->right);
    ast->right = NULL;


    size_t i = 0;
    while (data[i])
    {
        free(data[i]);
        i++;
    }
    free(data);
    free(ast);
}
