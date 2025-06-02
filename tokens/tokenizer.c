#include "../includes/minishell2.h"
#include "../includes/minishell.h"

t_token *create_token(char *value, t_token_type type) {
    t_token *new_token = malloc(sizeof(t_token));
    if (!new_token) return NULL;
    new_token->value = ft_strdup(value);
    new_token->type = type;
    new_token->next = NULL;
    return new_token;
}
void add_token(t_token **head, t_token *new_token) {
    if (!head || !new_token) return;
    if (!*head) {
        *head = new_token;
    } else {
        t_token *current = *head;
        while (current->next) {
            current = current->next;
        }
        current->next = new_token;
    }
}
void free_tokens(t_token *head) {
    t_token *current = head;
    while (current) {
        t_token *next = current->next;
        free(current->value);
        free(current);
        current = next;
    }
}
t_token *tokenize_input(const char *input) {
    t_token *head = NULL;
    const char *ptr = input;
    char buffer[1024];
    int buf_index = 0;

    while (*ptr) {
        if (*ptr == '|') {
            if (buf_index > 0) {
                buffer[buf_index] = '\0';
                add_token(&head, create_token(buffer, TOKEN_WORD));
                buf_index = 0;
            }
            add_token(&head, create_token("|", TOKEN_PIPE));
        } else if (*ptr == '>') {
            if (buf_index > 0) {
                buffer[buf_index] = '\0';
                add_token(&head, create_token(buffer, TOKEN_WORD));
                buf_index = 0;
            }
            if (*(ptr + 1) == '>') {
                add_token(&head, create_token(">>", TOKEN_APPEND));
                ptr++;
            } else {
                add_token(&head, create_token(">", TOKEN_REDIRECT_OUT));
            }
        } else if (*ptr == '<') {
            if (buf_index > 0) {
                buffer[buf_index] = '\0';
                add_token(&head, create_token(buffer, TOKEN_WORD));
                buf_index = 0;
            }
            add_token(&head, create_token("<", TOKEN_REDIRECT_IN));
        } else if (*ptr == '\n' || *ptr == ' ') {
            if (buf_index > 0) {
                buffer[buf_index] = '\0';
                add_token(&head, create_token(buffer, TOKEN_WORD));
                buf_index = 0;
            }
        } else {
            buffer[buf_index++] = *ptr;
        }
        ptr++;
    }

    if (buf_index > 0) {
        buffer[buf_index] = '\0';
        add_token(&head, create_token(buffer, TOKEN_WORD));
    }

    return head;
}
t_token *tokenize(const char *input) {
    if (!input || !*input) return NULL;

    // Check for unclosed quotes
    if (unclosed_quotes(input)) {
        fprintf(stderr, "minishell: unclosed quotes\n");
        return NULL;
    }

    // Tokenize the input
    t_token *tokens = tokenize_input(input);
    if (!tokens) {
        fprintf(stderr, "minishell: error tokenizing input\n");
        return NULL;
    }

    return tokens;
}
void print_tokens(t_token *tokens) {
    t_token *current = tokens;
    while (current) {
        printf("Token: %s, Type: %d\n", current->value, current->type);
        current = current->next;
    }
}
