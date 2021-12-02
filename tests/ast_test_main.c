#include "../src/lexer/lexer.h"
#include <stdio.h>

int main(void)
{
    char *sample_text = "hello there boys";
    struct lexer *lex = lexer_new(sample_text);

    struct token *token = lexer_pop(lex);
    printf("The tokens are: \n");
    while (token)
    {
        printf("[%s]\n", token->value);
        token = lexer_pop(lex);
    }
    return 0;
}
