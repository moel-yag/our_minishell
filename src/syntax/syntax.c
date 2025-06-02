#include "../../includes/minishell.h"

bool    check_syntax(const char *input)
{
    // int i = 0;
    if (!unclosed_quotes(input) || !pipe_syntax(input)
    || !file_syntax(input))
        return false;
    // while (input[i])
    // {
    //     if (input[i] == '|' && unclosed_quotes(input) == 0 && pipe_syntax(input))
    //         return 2;
    //     if ((input[i] == '<' || input[i] == '>') && unclosed_quotes(input) == 0
    //         && file_syntax(input))
    //         return 2;
    //     i++;
    // }
    // return 0;
    return true;
}
