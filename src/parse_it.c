#include "../includes/minishell.h"

t_command *parse_input(char *input, char **env)
{
    if (!input || !*input)
    {
        fprintf(stderr, "minishell: syntax error: empty input\n");
        return NULL;
    }
    t_command *cmd;
    char *processed_input;
    // First check for unclosed quotes
    if (has_unclosed_quotes(input))
    {
        fprintf(stderr, "minishell: syntax error: unclosed quotes\n");
        return NULL;
    }

    // Process quotes in the input
    processed_input = process_quotes(input);
    if (!processed_input)
    {
        fprintf(stderr, "minishell: error processing quotes\n");
        return NULL;
    }

    if (strstr(processed_input, "<<"))
    {
        handle_heredoc(processed_input, env);
        free(processed_input);
        return (NULL);
    }
    else if (strstr(processed_input, ">>"))
    {
        handle_append_redirection(processed_input, env);
        free(processed_input);
        return (NULL);
    }
    else if (strstr(processed_input, ">"))
    {
        handle_output_redirection(processed_input, env);
        free(processed_input);
        return (NULL);
    }
    else if (strstr(processed_input, "<"))
    {
        handle_input_redirection(processed_input, env);
        free(processed_input);
        return (NULL);
    }

    cmd = create_command(processed_input);
    free(processed_input);
    if (!cmd)
        return (NULL);
    return (cmd);
}

t_command *parse_command(char *command)
{
    t_command *cmd = malloc(sizeof(t_command));
    if (!cmd)
    {
        perror("malloc");
        return NULL;
    }

    char **args = ft_split(command, ' ');
    if (!args)
    {
        free(cmd);
        perror("ft_split");
        return NULL;
    }

    cmd->command = ft_strdup(args[0]);
    cmd->arguments = args;
    cmd->full_command = ft_strdup(command);

    return cmd;
}