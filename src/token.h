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
    TOKEN_IF, /*  if  */
    TOKEN_THEN, /*  then  */
    TOKEN_ELIF, /*   elif  */
    TOKEN_ELSE, /*  else  */
    TOKEN_FI, /*  fi  */
    TOKEN_PV, /*  Point Virgule  ;   */
    TOKEN_NL, /*  New Line       \n  */
    TOKEN_SQ, /*  Simple Quote   '   */
    TOKEN_OTHER, /*  words  */
};
