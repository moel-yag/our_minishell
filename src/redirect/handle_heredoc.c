#include "../includes/minishell.h"

static char *get_heredoc_filename(void)
{
    static int counter = 0;
    char *filename;
    char *num;

    num = ft_itoa(counter++);
    filename = ft_strjoin("/tmp/minishell_heredoc_", num);
    free(num);
    return filename;
}

void handle_heredoc(char *command, char **env)
{
    char **parts = ft_split(command, '<');
    if (!parts || !parts[1])
    {
        fprintf(stderr, "minishell: syntax error near unexpected token `<<`\n");
        free_split(parts);
        return;
    }

    char *cmd = parts[0];
    char *delimiter = parts[1];

    // Trim whitespace
    cmd = ft_strtrim(cmd, " ");
    delimiter = ft_strtrim(delimiter, " ");

    // Create temporary file for heredoc
    char *temp_file = get_heredoc_filename();
    int fd = open(temp_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0)
    {
        perror("minishell");
        free(temp_file);
        free_split(parts);
        return;
    }

    // Read input until delimiter
    char *line;
    while (1)
    {
        line = readline("> ");
        if (!line)
        {
            // Handle Ctrl+D
            break;
        }
        if (ft_strcmp(line, delimiter) == 0)
        {
            free(line);
            break;
        }
        write(fd, line, strlen(line));
        write(fd, "\n", 1);
        free(line);
    }
    close(fd);

    // Now execute the command with the heredoc file as input
    int stdin_backup = dup(STDIN_FILENO);
    fd = open(temp_file, O_RDONLY);
    if (fd < 0)
    {
        perror("minishell");
        free(temp_file);
        free_split(parts);
        return;
    }

    dup2(fd, STDIN_FILENO);
    close(fd);

    t_command *parsed_cmd = parse_command(cmd);
    shell_loop(parsed_cmd, env);
    free_command(parsed_cmd);

    dup2(stdin_backup, STDIN_FILENO);
    close(stdin_backup);
    unlink(temp_file); // Remove the temporary file
    free(temp_file);
    free(cmd);
    free(delimiter);
    free(temp_file);
    free_split(parts);
}