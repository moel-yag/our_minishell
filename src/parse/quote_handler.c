#include "../includes/minishell.h"

static char *process_quoted_string(const char *input, int *i, char quote_type)
{
    int start = *i + 1; // Skip the opening quote
    int len = 0;

    // Find the closing quote
    while (input[*i + 1] && input[*i + 1] != quote_type)
    {
        (*i)++;
        len++;
    }

    if (!input[*i + 1])
    {
        // No closing quote found
        return NULL;
    }

    // Extract the quoted content
    char *result = malloc(len + 1);
    if (!result)
        return NULL;

    strncpy(result, input + start, len);
    result[len] = '\0';

    (*i)++; // Move past the closing quote
    return result;
}

char *process_quotes(const char *input)
{
    if (!input)
        return NULL;

    char *result = malloc(strlen(input) + 1);
    if (!result)
        return NULL;

    int i = 0;
    int j = 0;
    bool in_single = false;
    bool in_double = false;

    while (input[i])
    {
        if (input[i] == '\'' && !in_double)
        {
            if (!in_single)
            {
                // Start of single quote
                char *quoted = process_quoted_string(input, &i, '\'');
                if (!quoted)
                {
                    free(result);
                    return NULL; // Unclosed quote
                }
                strcpy(result + j, quoted);
                j += strlen(quoted);
                free(quoted);
            }
            in_single = !in_single;
        }
        else if (input[i] == '"' && !in_single)
        {
            if (!in_double)
            {
                // Start of double quote
                char *quoted = process_quoted_string(input, &i, '"');
                if (!quoted)
                {
                    free(result);
                    return NULL; // Unclosed quote
                }
                strcpy(result + j, quoted);
                j += strlen(quoted);
                free(quoted);
            }
            in_double = !in_double;
        }
        else
        {
            result[j++] = input[i];
        }
        i++;
    }

    result[j] = '\0';
    return result;
}

bool has_unclosed_quotes(const char *input)
{
    bool in_single = false;
    bool in_double = false;
    int i = 0;

    while (input[i])
    {
        if (input[i] == '\'' && !in_double)
            in_single = !in_single;
        else if (input[i] == '"' && !in_single)
            in_double = !in_double;
        i++;
    }

    return in_single || in_double;
}