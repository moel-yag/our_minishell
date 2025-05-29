#include "../includes/minishell.h"

void handle_input_redirection(char *command, char **env)
{
    char **parts = ft_split(command, '<');
    if (!parts || !parts[1])
    {
        fprintf(stderr, "minishell: syntax error near unexpected token `<`\n");
        free_split(parts);
        return;
    }

    char *cmd = parts[0];
    char *file = parts[1];

    // Trim whitespace
    cmd = ft_strtrim(cmd, " ");
    file = ft_strtrim(file, " ");

    int fd = open(file, O_RDONLY);
    if (fd < 0)
    {
        perror("minishell");
        free_split(parts);
        return;
    }

    int stdin_backup = dup(STDIN_FILENO);
    dup2(fd, STDIN_FILENO);
    close(fd);

    t_command *parsed_cmd = parse_command(cmd);
    shell_loop(parsed_cmd, env);
    free_command(parsed_cmd);

    dup2(stdin_backup, STDIN_FILENO);
    close(stdin_backup);
    free_split(parts);
}