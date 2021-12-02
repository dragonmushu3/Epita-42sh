#include "../ast/ast.h"
#include <err.h>
#include <unistd.h>
#include <sys/wait.h>

static void execute_in_child(char **args)
{
    int pid = fork();
    if (pid == 0)
    {
        if (execvp(args[0], args) == -1)
            _exit(127);
    }
    else
    {
        int status = 0;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
        {
            if (WEXITSTATUS(status) == 127)
                warnx("%s: command not found", args[0]);
        }
    }
}
/*need to check for builtins*/

void exec_ast(struct ast *ast)
{
    if (ast == NULL)
        return;

    if (ast->type == AST_SIMPLE_COMM)
    {
        /*this could be put in a sub_function called exec_ast_simple_comm*/
        /*check if it's echo or other built-in or it won't work*/
        /*fix me*/
        execute_in_child(ast->data);
    }
    else if (ast->type == AST_COMM)
    {
        /*this could be put in a sub_function called exec_ast_comm */
        if (!ast->children)
        {
            warn("This command has no simple_commands attached to it! This shouldn't happen...");
        }
        else
        {
            size_t i = 0;
            while (ast->children[i])
            {
                execute_in_child(ast->children[i]->data);
                i++;
            }
        }
    }
    else
    {
        /*other node types when implemented*/
    }
}
