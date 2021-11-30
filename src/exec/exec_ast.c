#include "ast.h"
#include <err.h>
#include <unistd.h>
#include <sys/wait.h>

static void execute_in_child(char **args)
{
    int pid = fork();
    if (pid == 0)
    {
        if (execvp(args[0], args) == -1)
            err(127, "Couldn't exec");
    }
    else
    {
        int status = 0;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
        {
            if (WEXITSTATUS(status) == 127)
                warn("Child couldn't execute, error code %d", WEXITSTATUS(status));
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
        /*check if it's echo or other built-in or it won't work*/
        /*fix me*/
        execute_in_child(ast->data);
    }
    else
    {
        /*other node types when implemented*/
        return;
    }
}
