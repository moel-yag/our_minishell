#include "../includes/parsing.h"
#include "../includes/minishell.h"
#include "../includes/minishell2.h"

void    print_ast(t_ast *ast)
{
    int i = 0;
    if (!ast)
    {
        printf("AST is empty.\n");
        return;
    }
    while (ast)
    {
        printf("Command %d:\n", i++);
        printf("\t- Arguments: \n");
        t_list *arg = ast->args;
        while (arg)
        {
            printf("\t\t- %s\n", (char *)arg->content);
            arg = arg->next;
        }
        printf("\t- Redirections: \n");
        t_list *redir = ast->redirections;
        while (redir)
        {
            printf("\t\t- %s\n", (char *)redir->content);
            redir = redir->next;
        }
        printf("\n");
        ast = ast->next;
    }
}

void    test_parsing(const char *input, const char *test_name)
{
    printf("\n=== Testing: %s ===\n", test_name);
    printf("Input: \"%s\"\n", input);

    t_token *tokens = tokenize(input);
    if (!tokens)
    {
        printf("Tokenization failed!\n");
        return;
    }
    t_ast *ast = parser(input);
    if (!ast)
    {
        printf("Parsing failed!\n");
        free_tokens(tokens);
        return;
    }
    print_ast(ast);
    // free_ast(ast);
    free_tokens(tokens);
    printf("================================================\n");
}

int main(void)
{
    // Test 1: Basic command
    printf("=== 1 - Testing Basic Command ===\n");
    test_parsing("ls'hello'     'world' ||<<<<>>>>", "Basic command");

    // Test 2: Command with arguments
    printf("=== 2 - Testing Command with Arguments ===\n");
    test_parsing("ls -la", "Command with arguments");

    // Test 3: Command with pipe
    printf("=== 3 - Testing Command with Pipe ===\n");
    test_parsing("ls | grep test", "Command with pipe");

    // Test 4: Command with output redirection
    printf("=== 4 - Testing Command with Output Redirection ===\n");
    test_parsing("ls > output.txt", "Command with output redirection");
    
    // Test 5: Command with input redirection
    printf("=== 5 - Testing Command with Input Redirection ===\n");
    test_parsing("cat < input.txt", "Command with input redirection");

    // Test 6: Command with append redirection
    printf("=== 6 - Testing Command with Append Redirection ===\n");
    test_parsing("echo 'Hello' >> output.txt", "Command with append redirection");

    // Test 7: Command with heredoc
    printf("=== 7 - Testing Command with Heredoc ===\n");
    test_parsing("cat << EOF Hello World EOF", "Command with heredoc");

    // Test 8: Complex command with multiple redirections and pipes
    printf("=== 8 - Testing Complex Command with Pipe and Redirection ===\n");
    test_parsing("ls -la | grep test > output.txt", "Complex command with pipe and redirection");

    // Test 9: Multiple spaces
    printf("=== 9 - Testing Multiple Spaces ===\n");
    test_parsing("ls    -la    |    grep    test", "Multiple spaces");

    // Test 10: Empty input
    printf("=== 10 - Testing Empty Input ===\n");
    test_parsing("", "Empty input");

    return 0;
}
