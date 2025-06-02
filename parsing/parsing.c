#include "../includes/parsing.h"
#include "../includes/minishell.h"
#include "../includes/minishell2.h"

// char * ft_token_gettype(t_token_type type);
t_ast *create_ast_node(void)
{
    t_ast *node = malloc(sizeof(t_ast));
    if (!node)
        return NULL;
    node->args = NULL;
    node->redirections = NULL;
    node->next = NULL;
    return node;
}
void free_ast(t_ast *ast)
{
    if (!ast)
        return;
    // ft_lstclear(&ast->args, free);
    // ft_lstclear(&ast->redirections, free);
    t_ast *next_node = ast->next;
    while (next_node)
    {
        t_ast *temp = next_node;
        next_node = next_node->next;
        free_ast(temp);
    }
    ast->next = NULL; // Clear the next pointer to avoid dangling pointers
    // Free the current node
    // Note: We don't free ast->args and ast->redirections here because they are lists
    // and will be freed by ft_lstclear.
    // If they were allocated with malloc, we should free them here.
    // However, since they are lists, we assume they are managed by the list functions.
    // If args and redirections were allocated with malloc, we should free them here.
    free(ast);
}
void add_ast_node(t_ast **head, t_ast *new_node)
{
    if (!head || !new_node)
        return;
    if (!*head)
    {
        *head = new_node;
    }
    else
    {
        t_ast *current = *head;
        while (current->next)
            current = current->next;
        current->next = new_node;
    }
}
void free_ast_list(t_ast *head)
{
    t_ast *current = head;
    while (current)
    {
        t_ast *next = current->next;
        free_ast(current);
        current = next;
    }
}
// void print_ast(t_ast *ast)
// {
//     if (!ast)
//         return;
//     printf("AST Node:\n");
//     printf("Arguments: ");
//     // print_list(ast->args);
//     printf("Redirections: ");
//     // print_list(ast->redirections);
//     if (ast->next)
//         printf("Next node exists.\n");
//     else
//         printf("No next node.\n");
// }

bool ft_token_is_redirection(t_token_type type)
{
    return (type == TOKEN_REDIRECT_IN || type == TOKEN_REDIRECT_OUT || type == TOKEN_APPEND 
            || type == TOKEN_HEREDOC);
}

void    ft_lst_push(t_list **head, char *value)
{
    t_list *new_node = ft_lstnew(value);
    if (!new_node)
        return;
    ft_lstadd_back(head, new_node);
}

t_ast *parser(const char *input)
{
    t_token *tokens = tokenize(input);
    if (!tokens)
    {
        fprintf(stderr, "Error tokenizing input.\n");
        return NULL;
    }

    t_ast *ast = NULL;
    t_ast *curr = create_ast_node();
    if (!curr)
    {
        fprintf(stderr, "Error creating AST node.\n");
        free_tokens(tokens);
        return NULL;
    }
    ast = curr; // Initialize the AST with the first node
    t_token *current = tokens;
    while (current)
    {     
        printf("Current token: Type: %s, Value: %s\n", ft_token_gettype(current->type), current->value);  
        if (current->type == TOKEN_EOF)
            break;
        else if (current->type == TOKEN_WORD)
            ft_lst_push(&curr->args, current->value);
        else if (ft_token_is_redirection(current->type))
        {
            if (current->next && current->next->type == TOKEN_WORD)
            {
                char *redirection = current->next->value;
                // redirection = current->next->value;
                ft_lst_push(&curr->redirections, redirection);
                current = current->next; // Skip the next word as it's already processed
            }
            else
            {
                ft_lst_push(&curr->redirections, current->value);
                printf("Redirection without target%s\n", current->value);
                free_ast(ast);
                free_tokens(tokens);
                return NULL;
            }
        }
        else if (current->type == TOKEN_PIPE)
        {
            // next pipe should be a word or a redirection
            if (current->next->type != TOKEN_WORD && !ft_token_is_redirection(current->next->type))
            {
                fprintf(stderr, "Syntax error: Pipe not followed by a command or redirection.\n");
                free_ast(ast);
                free_tokens(tokens);
                return NULL;
            }
            t_ast *new_node = create_ast_node();
            if (!new_node)
            {
                fprintf(stderr, "Error creating new AST node for pipe.\n");
                free_ast(ast);
                free_tokens(tokens);
                return NULL;
            }
            add_ast_node(&ast, new_node);
            curr = new_node; // Move to the new node
        }
 
        else
        {
            fprintf(stderr, "Unexpected token type: %d\n", current->type);
            free_ast(ast);
            free_tokens(tokens);
            return NULL;
        }
        current = current->next;
    }
    return ast;
}
