#include "../includes/minishell.h"

void free_commands(t_command *head) {
    t_command *temp;
    if (!head)
        return;
    while (head) {
        temp = head;
        free(head->command);
        free(head->full_command);
        free_split(head->arguments);
        head = temp->next;
        free(temp);
    }
}
