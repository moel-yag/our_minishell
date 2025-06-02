#include "../includes/minishell.h"

t_command *create_command(char *cmd) {
    if (!cmd || ft_strlen(cmd) == 0) {
        fprintf(stderr, "minishell: syntax error: empty command\n");
        return NULL;
    }
    int i = 0;
    t_command *new_node = malloc(sizeof(t_command));
    if (!new_node)
        return NULL;
    new_node->full_command = ft_strdup(cmd);
    char **tokens = ft_split(cmd, ' ');
    if (!tokens)
    {
        free(new_node);
        return (NULL);
    }
    // while (tokens[i])
    // {    
    //     // if (ft_strlen(tokens[i]) == 0) {
    //     //     free_split(tokens);
    //     //     free(new_node->full_command);
    //     //     free(new_node);
    //     //     return NULL;
    //     // }
    //     printf("tokens[%d]: %s\n", i, tokens[i]);
    //     i++;
    // }
    int len = get_len(tokens);
    new_node->command = ft_strdup(tokens[0]);
    // if (!new_node->command) {
    //     free(new_node->full_command);
    //     free(new_node);
    //     free_split(tokens);
    //     return NULL;
    // }
    new_node->arguments = NULL;
    if (len > 1)
    {
        new_node->arguments = malloc(sizeof(char *) * (ft_split_size(tokens) + 1));
        if (!new_node->arguments){
            free(new_node->full_command);
            free(new_node);
            free_split(tokens);
            return NULL;
        }
        while (tokens[i]) {
            new_node->arguments[i] = ft_strdup(tokens[i]);
            i++;
        }
        new_node->arguments[i] = NULL;
    }
    new_node->next = NULL;
    free_split(tokens);
    return (new_node);
}
