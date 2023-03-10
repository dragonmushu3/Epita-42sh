#include "lexer.h"

#include <assert.h>
#include <ctype.h>
#include <err.h>
#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct lexer *lexer_new(const char *input)
{
    struct lexer *retourne = calloc(512, sizeof(struct lexer));
    if (!retourne)
    {
        errx(1, "error creation struct lexer");
        return NULL;
    }
    retourne->input = input;
    retourne->pos = 0;
    retourne->current_tok = NULL;
    /*initialise lors de l'usage*/
    return retourne;
}

void lexer_free(struct lexer *lexer)
{
    /*le parseur s'occupe de changer les pos du suivant automatiquement;*/
    free(lexer);
    return;
}

static char *segment_input(struct lexer *lexer, size_t start, size_t end)
{
    char *str = malloc(sizeof(char) * (end - start + 1));
    if (!str)
    {
        return NULL;
    }
    for (size_t i = start; i < end; i++)
    {
        str[i - start] = lexer->input[i];
    }
    str[end - start] = '\0';
    return str;
}

int my_isspace(char c)
{
    return c == ';' || c == 32 || c == '\t' || c == '\r' || c == '\f' || c == '\v' || c == '\0' || c == '\n';
}
int is_sp_comm_end(char c)
{
    return c == ';' || c == '\n' || c == '\0';
}

struct token *lexer_peek(struct lexer *lexer)
{
    /*returns the same token if one is already present */
    if (lexer->current_tok)
    {
        return lexer->current_tok;
    }


    struct token *toke = calloc(1, sizeof(struct token));
    if (!toke)
    {
        return NULL;
    }
    while ((lexer->input[lexer->pos] == ' ')
           || (lexer->input[lexer->pos] == '\t'))
    {
        lexer->pos = lexer->pos + 1;
    }

    size_t word_start = 0;
    size_t word_end = 0;
    if (!is_sp_comm_end(lexer->input[lexer->pos]))
    {
        word_start = lexer->pos;
        while (!my_isspace(lexer->input[lexer->pos]))
        {
            lexer->pos++;
        }
        word_end = lexer->pos;
    }
    else
    {
        if (lexer->input[lexer->pos] == '\0')
        {
            word_start = lexer->pos;
            word_end = lexer->pos + 1;
        }
        else
        {
            word_start = lexer->pos;
            word_end = ++lexer->pos;
        }
    }
    char *value = segment_input(lexer, word_start, word_end);

    /* compare the word with eventually a token*/
    if (strcmp("if", value) == 0)
    {
        toke->type = TOKEN_IF;
        toke->value = value;
    }
    else if (strcmp("else", value) == 0)
    {
        toke->type = TOKEN_ELSE;
        toke->value = value;
    }
    else if (strcmp("elif", value) == 0)
    {
        toke->type = TOKEN_ELIF;
        toke->value = value;
    }
    else if (strcmp("fi", value) == 0)
    {
        toke->type = TOKEN_FI;
        toke->value = value;
    }
    else if (strcmp("'", value) == 0)
    {
        toke->type = TOKEN_SQ;
        toke->value = value;
    }
    else if (*value ==  ';')
    {
        toke->type = TOKEN_PV;
        toke->value = value;
    }
    else if (strcmp("then", value) == 0)
    {
        toke->type = TOKEN_THEN;
        toke->value = value;
    }
    else if (*value == '\n')
    {
        toke->type = TOKEN_NL;
        toke->value = value;
    }
    else if (*value == '\0')
    {
        toke->type = TOKEN_EOF;
        toke->value = value;
    }
    else
    {
        toke->type = TOKEN_OTHER;
        toke->value = value;
    }
    lexer->current_tok = toke;
    return toke;
}

struct token *lexer_pop(struct lexer *lexer)
{
    struct token *toke = NULL;
    if (!lexer->current_tok)
    {
        toke = lexer_peek(lexer);
        lexer->current_tok = NULL;
        return toke;
    }
    toke = calloc(1, sizeof(struct token));
    if (!toke)
    {
        return NULL;
    }
    toke->type = lexer->current_tok->type;
    if (toke->type == TOKEN_OTHER)
    {
        toke->value = lexer->current_tok->value;
    }
    free(lexer->current_tok);
    lexer->current_tok = NULL;
    return toke;
}
