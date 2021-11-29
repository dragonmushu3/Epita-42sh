#include "parser.h"
#include "alloc.h"

#include <err.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

static enum parser_status parse_if(struct ast **res, struct lexer *lexer);
static enum parser_status parse_pv(struct ast **res, struct lexer *lexer);
static enum parser_status parse_fi(struct ast **res, struct lexer *lexer);
static enum parser_status parse_then(struct ast **res, struct lexer *lexer);
static enum parser_status parse_if(struct ast **res, struct lexer *lexer);
static enum parser_status parse_sq(struct ast **res, struct lexer *lexer);

static enum parser_status handle_parse_error(enum parser_status status,
                                             struct ast **res)
{
    warnx("unexpected token");
    ast_free(*res);
    *res = NULL;
    return status;
}

enum parser_status parse_simple_comm(struct ast **res, struct lexer *lexer)
{
    struct token *tok = lexer_peek(lexer);
    if (tok->type != TOKEN_OTHER)
    {
        return PARSER_UNEXPECTED_TOKEN;
    }
    struct ast *sp_comm = ast_new(AST_SIMPLE_COMM);
    size_t data_size = sizeof(char*) * 1;
    size_t data_index = 0;
    sp_comm->data = malloc(data_size);
    sp_comm->data[0] = tok->value;
    token_free(lexer_pop(lexer));
    while (true)
    {
        tok = lexer_peek(lexer);
        if (tok->type == TOKEN_OTHER)
        {
            data_size += sizeof(char*) * 1;
            sp_comm->data = realloc(sp_comm->data, data_size);
            data_index++;
            sp_comm->data[data_index] = tok->value;
            token_free(lexer_pop(lexer));
        }
        else if (tok->type == TOKEN_EOF || tok->type == TOKEN_NL)
        {
            data_size += sizeof(char*) * 1;
            sp_comm->data = realloc(sp_comm->data, data_size);
            data_index++;
            sp_comm->data[data_index] = NULL;
            *res = sp_comm;
            free(tok->value);
            token_free(lexer_pop(lexer));
            return PARSER_OK;
        }
    }
}



/* from TOKEN_LESS_PRIORITY
 *  to  TOKEN_MORE_PRIORITY*/

/**
 * \brief Parse either an expression, or nothing
 *
 * input:  EOF  |  exp EOF
 */
enum parser_status parse(struct ast **res, struct lexer *lexer)
{
    // If we're at the end of file, there's no input
    struct token *tok = lexer_peek(lexer);
    if (tok->type == TOKEN_EOF)
    {
        *res = NULL;
        return PARSER_OK;
    }

    // try to parse an expression. if an error occured, free the
    // produced ast and return the same error code
    enum parser_status status = parse_simple_comm(res, lexer);
    if (status != PARSER_OK)
        return handle_parse_error(status, res);
    return PARSER_OK;

    // once parsing the expression is done, we should have
    // reached the end of file.
    if (lexer_peek(lexer)->type == TOKEN_EOF)
        return PARSER_OK;

    // if we didn't reach the end of file, it's an error
    return handle_parse_error(PARSER_UNEXPECTED_TOKEN, res);
}

/**
 * \brief Parse sexp expressions separated by + and -
 *
 * exp:      sexp  (' ; ' sexp)*
 */
static enum parser_status parse_pv(struct ast **res, struct lexer *lexer)
{
    enum parser_status status = parse_fi(res, lexer);
    if (status != PARSER_OK)
        return status;

    while (true)
    {
        // have a look at the type of the next token. Stop if not interested
        struct token *tok = lexer_peek(lexer);
        if (tok->type != TOKEN_PV)
            return PARSER_OK;

        // make the new ast node and discard the token
        struct ast *new_ast;
        if (tok->type == TOKEN_PV)
            new_ast = ast_new(AST_PV);
        token_free(lexer_pop(lexer));

        // attach the new node to the ast
        new_ast->left = *res;
        *res = new_ast;

        // parse the stuff at the right
        if ((status = parse_fi(&new_ast->right, lexer)) != PARSER_OK)
            return status;
    }
}

/**
 * \brief Parse sexp expressions separated by + and -
 *
 * exp:      sexp  ('fi' sexp)*
 */
static enum parser_status parse_fi(struct ast **res, struct lexer *lexer)
{
    enum parser_status status = parse_then(res, lexer);
    if (status != PARSER_OK)
        return status;

    while (true)
    {
        // have a look at the type of the next token. Stop if not interested
        struct token *tok = lexer_peek(lexer);
        if (tok->type != TOKEN_FI)
            return PARSER_OK;

        // make the new ast node and discard the token
        struct ast *new_ast;
        if (tok->type == TOKEN_FI)
            new_ast = ast_new(AST_FI);
        token_free(lexer_pop(lexer));

        // attach the new node to the ast
        new_ast->left = *res;
        *res = new_ast;

        // parse the stuff at the right
        if ((status = parse_then(&new_ast->right, lexer)) != PARSER_OK)
            return status;
    }
}

/**
 * \brief Parse sexp expressions separated by + and -
 *
 * exp:      sexp  ('then' sexp)*
 */
static enum parser_status parse_then(struct ast **res, struct lexer *lexer)
{
    enum parser_status status = parse_if(res, lexer);
    if (status != PARSER_OK)
        return status;

    while (true)
    {
        // have a look at the type of the next token. Stop if not interested
        struct token *tok = lexer_peek(lexer);
        if (tok->type != TOKEN_THEN)
            return PARSER_OK;

        // make the new ast node and discard the token
        struct ast *new_ast;
        if (tok->type == TOKEN_THEN)
            new_ast = ast_new(AST_THEN);
                // attach the new node to the ast
        new_ast->left = *res;
        *res = new_ast;

        // parse the stuff at the right
        if ((status = parse_if(&new_ast->right, lexer)) != PARSER_OK)
            return status;
    }
}



/**
 * \brief Parse sexp expressions separated by + and -
 *
 * exp:      sexp  ('if' sexp | 'elif' sexp | 'else' sexp)*
 */
static enum parser_status parse_if(struct ast **res, struct lexer *lexer)
{
    enum parser_status status = parse_sq(res, lexer);
    if (status != PARSER_OK)
        return status;

    while (true)
    {
        // have a look at the type of the next token. Stop if not interested
        struct token *tok = lexer_peek(lexer);
        if (tok->type != TOKEN_IF && tok->type != TOKEN_ELIF && tok->type != TOKEN_ELSE)
            return PARSER_OK;

        // make the new ast node and discard the token
        struct ast *new_ast;
        if (tok->type == TOKEN_IF)
            new_ast = ast_new(AST_IF);
        // ajouter le elif comme branche du if s'il existe, sinon err////////
        if (tok->type == TOKEN_ELIF)
            new_ast = ast_new(AST_ELIF);
        else
            new_ast = ast_new(AST_ELSE);
        token_free(lexer_pop(lexer));

        // attach the new node to the ast
        new_ast->left = *res;
        *res = new_ast;

        // parse the stuff at the right
        if ((status = parse_sq(&new_ast->right, lexer)) != PARSER_OK)
            return status;
    }
}

/**
 * \brief Parse sexp expressions separated by + and -
 *
 * exp:      sexp  (" ' " sexp)*
 */
static enum parser_status parse_sq(struct ast **res, struct lexer *lexer)
{
    enum parser_status status = parse_pv(res, lexer);
    if (status != PARSER_OK)
        return status;

    while (true)
    {
        // have a look at the type of the next token. Stop if not interested
        struct token *tok = lexer_peek(lexer);
        if (tok->type != TOKEN_SQ)
            return PARSER_OK;

        // make the new ast node and discard the token
        struct ast *new_ast;
        if (tok->type == TOKEN_SQ)
            new_ast = ast_new(AST_SQ);
        token_free(lexer_pop(lexer));

        // attach the new node to the ast
        new_ast->left = *res;
        *res = new_ast;

        // parse the stuff at the right
        if ((status = parse_pv(&new_ast->right, lexer)) != PARSER_OK)
            return status;
    }
}

///////renommer fonction
