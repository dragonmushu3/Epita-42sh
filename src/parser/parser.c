#include "parser.h"
#include "../utils/alloc.h"

#include <err.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

static enum parser_status parse_simple_comm(struct ast **res, struct lexer *lexer);
/*besoin redefinition for Rule if:
 * if SIMPLE_COMM ;then SIMPLECOM*/
/*static enum parser_status parse_pv(struct ast **res, struct lexer *lexer);
static enum parser_status parse_fi(struct ast **res, struct lexer *lexer);
static enum parser_status parse_then(struct ast **res, struct lexer *lexer);
static enum parser_status parse_if(struct ast **res, struct lexer *lexer);
static enum parser_status parse_sq(struct ast **res, struct lexer *lexer);
*/

static enum parser_status handle_parse_error(enum parser_status status,
                                             struct ast **res)
{
    ast_free(*res);
    *res = NULL;
    return status;
}

/**
 * \brief Parse simple_command formed of redirections and words
 *
 * simple_command: (prefix)+ | (prefix)* (element)+
 */
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
        if (tok->type == TOKEN_EOF || tok->type == TOKEN_NL || tok->type == TOKEN_PV)
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
        else
        {
            data_size += sizeof(char*) * 1;
            sp_comm->data = realloc(sp_comm->data, data_size);
            data_index++;
            sp_comm->data[data_index] = tok->value;
            token_free(lexer_pop(lexer));
        }
    }
}

/**
 * \brief Parse sexp expressions separated by + and -
 *
 * exp:      sexp  ('if' sexp | 'elif' sexp | 'else' sexp)*
 */
static enum parser_status parse_if(struct ast **res, struct lexer *lexer)
{
    struct token *tok = NULL;
    struct ast *ast_if = ast_new(AST_IF);
    ast_if->children = malloc(sizeof(struct ast) * 4);
    ast_if->children[3] = NULL;
    if (!ast_if->children)
    {
        return 0;
    }

    /*!!!! change this function to parse list_command*/
    if (parse_simple_comm(res, lexer) == PARSER_OK)
    {
        ast_if->children[0] = *res;
    }
    else
    {
        /*if parse_simple_comm failed*/
        /*it should be sp coms jobs to print out relevant info*/
        return PARSER_UNEXPECTED_TOKEN;
    }
    tok = lexer_peek(lexer);
    if (tok->type == TOKEN_THEN)
    {
        free(tok->value);
        token_free(lexer_pop(lexer));
        if (parse_simple_comm(res, lexer) == PARSER_OK)
        {
            ast_if->children[1] = *res;
        }
        else
        {
            /*if parse_simple_comm failed*/
            warnx("Incomplete input");
            return PARSER_UNEXPECTED_TOKEN;
        }
    }
    else
    {
        /*if there is no then*/
        warnx("Incomplete input");
        free(tok->value);
        token_free(lexer_pop(lexer));
        return PARSER_UNEXPECTED_TOKEN;
    }
    tok = lexer_peek(lexer);
    if (tok->type == TOKEN_ELSE)
    {
        free(tok->value);
        token_free(lexer_pop(lexer));
        if (parse_simple_comm(res, lexer) == PARSER_OK)
        {
            ast_if->children[2] = *res;
        }
        else
        {
            /*if parse_simple_comm failed*/
            return PARSER_UNEXPECTED_TOKEN;
        }
    }
    else if (tok->type == TOKEN_ELIF)
    {
        free(tok->value);
        token_free(lexer_pop(lexer));
        if (parse_if(res, lexer) == PARSER_OK)
        {
            ast_if->children[2] = *res;
            *res = ast_if;
            return PARSER_OK;
        }
        else
        {
            /*if parse_if failed*/
            return PARSER_UNEXPECTED_TOKEN;
        }
    }
    else if (tok->type == TOKEN_FI)
    {
        ast_if->children[2] = NULL;
        free(tok->value);
        token_free(lexer_pop(lexer));
        *res = ast_if;
        return PARSER_OK;
    }
    else
    {
        /*if there is no fi*/
        warnx("syntax error near unexpected token '%s'", tok->value);
        free(tok->value);
        token_free(lexer_pop(lexer));
        return PARSER_UNEXPECTED_TOKEN;
    }

    /*we would only come here from an else*/
    tok = lexer_peek(lexer);
    if (tok->type == TOKEN_FI)
    {
        free(tok->value);
        token_free(lexer_pop(lexer));
        *res = ast_if;
        return PARSER_OK;
    }
    else
    {
        /*if there is no fi*/
        warnx("syntax error near unexpected token '%s'", tok->value);
        free(tok->value);
        token_free(lexer_pop(lexer));
        return PARSER_UNEXPECTED_TOKEN;
    }
    return PARSER_UNEXPECTED_TOKEN;
}


/**
 * \brief Parse sexp expressions separated by + and -
 *
 * shell_command: rule_if
 */
enum parser_status parse_shell_comm(struct ast **res, struct lexer *lexer)
{
    struct token *tok = lexer_peek(lexer);
    if (tok->type != TOKEN_IF)
    {
        /*this just means that it isn't a rule if*/
        return PARSER_UNEXPECTED_TOKEN;
    }
    else
    {
        free(tok->value);
        token_free(lexer_pop(lexer));
        return parse_if(res, lexer);
    }
}


/**
 * \brief Parse sexp expressions separated by + and -
 *
 * command: simple_command | shell_command
 *
 * temporarily command == simple_com
 */
static enum parser_status parse_command(struct ast **res, struct lexer *lexer)
{
    enum parser_status status = parse_simple_comm(res, lexer);
    if (status == PARSER_OK)
        return status;
    status = parse_shell_comm(res, lexer);
    if (status == PARSER_OK)
        return status;
    /*if the sp and shell comm each peeked*/
    if (lexer->current_tok)
    {
        free(lexer->current_tok->value);
    }
    token_free(lexer_pop(lexer));
    return PARSER_UNEXPECTED_TOKEN;
}



/**
 * \brief Parse sexp expressions separated by + and -
 *
 * list: command (';' command)* [';']
 *
 * temporarily command == simple_com
 */
static enum parser_status parse_list(struct ast **res, struct lexer *lexer)
{
    enum parser_status status = parse_command(res, lexer);
    if (status != PARSER_OK)
        return status;

    struct ast *list = ast_new(AST_LIST);
    size_t children_size = sizeof(struct ast);
    size_t child_index = 0;
    list->children = malloc(sizeof(struct ast) * 1);
    if (!list->children)
    {
        return 0;
    }
    list->children[child_index] = *res;
    while (true)
    {
        status = parse_command(res, lexer);
        if (status != PARSER_OK)
        {
            children_size += sizeof(struct ast);
            list->children = realloc(list->children, children_size);
            child_index++;
            list->children[child_index] = NULL;
            *res = list;
            return PARSER_OK;
        }
        children_size += sizeof(struct ast);
        list->children = realloc(list->children, children_size);
        child_index++;
        list->children[child_index] = *res;
    }
}



//
//
///**
// * \brief Parse sexp expressions separated by + and -
// *
// * pipeline: ['!'] command ('|' ('\n')* command)
// */
//static enum parser_status parse_pipeline(struct ast **res, struct lexer *lexer)
//{
//    enum parser_status status = parse_command(res, lexer);
//    if (status != PARSER_OK)
//        return status;
//
//    while (true)
//    {
//        // have a look at the type of the next token. Stop if not interested
//        struct token *tok = lexer_peek(lexer);
//        if (tok->type != TOKEN_PV)
//            return PARSER_OK;
//
//        // make the new ast node and discard the token
//        struct ast *new_ast;
//        if (tok->type == TOKEN_PV)
//            new_ast = ast_new(AST_PV);
//        token_free(lexer_pop(lexer));
//
//        // attach the new node to the ast
//        new_ast->left = *res;
//        *res = new_ast;
//
//        // parse the stuff at the right
//        if ((status = parse_fi(&new_ast->right, lexer)) != PARSER_OK)
//            return status;
//    }
//}
//
//
//
//
///**
// * \brief Parse sexp expressions separated by + and -
// *
// * and_or:  pipeline (('&&'|'||') ('\n')* pipeline)*
// */
//static enum parser_status parse_and_or(struct ast **res, struct lexer *lexer)
//{
//    enum parser_status status = parse_pipeline(res, lexer);
//    if (status != PARSER_OK)
//        return status;
//
//    while (true)
//    {
//        // have a look at the type of the next token. Stop if not interested
//        struct token *tok = lexer_peek(lexer);
//        if (tok->type != TOKEN_PV)
//            return PARSER_OK;
//
//        // make the new ast node and discard the token
//        struct ast *new_ast;
//        if (tok->type == TOKEN_PV)
//            new_ast = ast_new(AST_PV);
//        token_free(lexer_pop(lexer));
//
//        // attach the new node to the ast
//        new_ast->left = *res;
//        *res = new_ast;
//
//        // parse the stuff at the right
//        if ((status = parse_fi(&new_ast->right, lexer)) != PARSER_OK)
//            return status;
//    }
//}
//
//



/* from TOKEN_LESS_PRIORITY
 *  to  TOKEN_MORE_PRIORITY*/

/**
 * \brief Parse either an expression, or nothing
 *
 * input:  list '\n' | list EOF | '\n' | EOF
 */
enum parser_status parse(struct ast **res, struct lexer *lexer)
{
    // If we're at the end of file, there's no input
    struct token *tok = lexer_peek(lexer);
    if ((tok->type == TOKEN_EOF) ||  (tok->type == TOKEN_NL))
    {
        *res = NULL;
        free(tok->value);
        token_free(tok);
        return PARSER_OK;
    }

    // try to parse an list. if an error occured, free the
    // produced ast and return the same error code
    enum parser_status status = parse_list(res, lexer);
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
///**
// * \brief Parse sexp expressions separated by + and -
// *
// * exp:      sexp  (" ' " sexp)*
// */
//static enum parser_status parse_sq(struct ast **res, struct lexer *lexer)
//{
//    enum parser_status status = parse_pv(res, lexer);
//    if (status != PARSER_OK)
//        return status;
//
//    while (true)
//    {
//        // have a look at the type of the next token. Stop if not interested
//        struct token *tok = lexer_peek(lexer);
//        if (tok->type != TOKEN_SQ)
//            return PARSER_OK;
//
//        // make the new ast node and discard the token
//        struct ast *new_ast;
//        if (tok->type == TOKEN_SQ)
//            new_ast = ast_new(AST_SQ);
//        token_free(lexer_pop(lexer));
//
//        // attach the new node to the ast
//        new_ast->left = *res;
//        *res = new_ast;
//
//        // parse the stuff at the right
//        if ((status = parse_pv(&new_ast->right, lexer)) != PARSER_OK)
//            return status;
//    }
//}
