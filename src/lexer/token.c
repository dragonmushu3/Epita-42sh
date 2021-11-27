#include "token.h"

#include <err.h>
#include <stdlib.h>

#include "alloc.h"

struct token *token_new(enum token_comm type)
{
    struct token *new = zalloc(sizeof(struct token));
    new->type = type;
    return new;
}

void token_free(struct token *token)
{
    free(token->value);
    free(token);
    token = NULL;
}