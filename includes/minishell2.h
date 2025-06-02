#ifndef MINISHELL2_H
#define MINISHELL2_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../libft/libft.h"

typedef enum
{
    TOKEN_WORD, // Regular word or command
    TOKEN_PIPE,
    TOKEN_REDIRECT_IN,
    TOKEN_REDIRECT_OUT,
    TOKEN_APPEND,
    TOKEN_HEREDOC,
    TOKEN_EOF, // End of file or input
} t_token_type;

typedef struct s_token
{
    char *value;          // The value of the token
    t_token_type type;    // The type of the token (e.g., command, argument, operator)
    struct s_token *next; // Pointer to the next token in the list
} t_token;

// Token functions
t_token *create_token(char *value, t_token_type type);
void add_token(t_token **head, t_token *new_token);
void free_tokens(t_token *head);
void print_tokens(t_token *head);
t_token *tokenize_input(const char *input);
t_token *tokenize(const char *input);

#endif // MINISHELL2_H
