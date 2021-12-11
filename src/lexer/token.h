#pragma once

#include <unistd.h>

/*ou chercher la donne; argv(1)*/
enum token_lecture
{
    TOKEN_C, /*  -c  */
    TOKEN_SH, /* file.SH  */
};

/*est ce une commande ?*/
enum token_comm
{
    TOKEN_IONUMBER, /* 0 1 2 3 4 5 6 7 8 9 , using in redirection*/
    TOKEN_IF, /*  if  */
    TOKEN_THEN, /*  then  */
    TOKEN_ELIF, /*   elif  */
    TOKEN_ELSE, /*  else  */
    TOKEN_FI, /*  fi  */
    TOKEN_PV, /*  Point Virgule  ;   */
    TOKEN_NL, /*  New Line       \n  */
    TOKEN_SQ, /*  Simple Quote   '   */
    TOKEN_EOF, /* End Of File    \0  */
    TOKEN_NEGATION, /* not       !   */
    TOKEN_PIPE, /* or        |   */
    TOKEN_REDIRECTION, /* >  >>  <  <<  <<-  >&  <&  >|  <> */
    TOKEN_OTHER, /*  words  */
};
struct token
{
    enum token_comm type; ///< The kind of token
    char *value; ///< If word, it's string
};

struct token *token_new(enum token_comm type);
/*  brief Frees a token  */

void token_free(struct token *token);
