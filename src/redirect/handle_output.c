#include "../includes/minishell.h"

void handle_output_redirection(char *command, char **env) {
    char **parts = ft_split(command, '>');
    if (!parts || !parts[1]) {
        fprintf(stderr, "minishell: syntax error near unexpected token `>`\n");
        free_split(parts);
        return;
    }

    char *cmd = parts[0];
    char *file = parts[1];

    // Trim whitespace
    cmd = ft_strtrim(cmd, " ");
    file = ft_strtrim(file, " ");

    // Only use the part before the next pipe as the filename
    char *pipe_pos = ft_strchr(file, '|');
    if (pipe_pos)
    {
        *pipe_pos = '\0'; // Truncate at the pipe
        // Optionally trim again in case of trailing spaces
        char *tmp = file;
        file = ft_strtrim(file, " ");
        free(tmp);
    }

    int fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        perror("minishell");
        free_split(parts);
        return;
    }

    int stdout_backup = dup(STDOUT_FILENO);
    dup2(fd, STDOUT_FILENO);
    close(fd);

    t_command *parsed_cmd = parse_command(cmd);
    shell_loop(parsed_cmd, env);
    free_command(parsed_cmd);

    dup2(stdout_backup, STDOUT_FILENO);
    close(stdout_backup);
    free(cmd);
    free(file);
    free_split(parts);
}