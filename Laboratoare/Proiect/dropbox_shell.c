#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

pthread_mutex_t log_mutex;

// Funcție pentru executarea unei comenzi sistem
void execute_command(const char *command) {
    pthread_mutex_lock(&log_mutex); // Sincronizare pentru zona critică
    printf("Execut comanda: %s\n", command);
    system(command); // Rulează comanda folosind sistemul
    pthread_mutex_unlock(&log_mutex);
}

void print_help() {
    printf("Comenzi disponibile:\n");
    printf("  cp <sursă> <destinație>   - Copiază fișier\n");
    printf("  mv <sursă> <destinație>   - Mută fișier\n");
    printf("  mkdir <director>          - Creează director\n");
    printf("  rm <fișier/director>      - Șterge fișier sau director\n");
    printf("  exit                      - Ieșire din shell\n");
}

int main() {
    pthread_mutex_init(&log_mutex, NULL); // Inițializare mutex

    char input[512];
    while (1) {
        printf("dropbox-shell> ");
        if (fgets(input, sizeof(input), stdin) == NULL) break;

        // Elimină newline din input
        input[strcspn(input, "\n")] = 0;

        // Comandă `exit`
        if (strcmp(input, "exit") == 0) break;

        // Parsarea comenzii
        char command[512];
        if (strncmp(input, "cp ", 3) == 0) {
            snprintf(command, sizeof(command), "dbxcli cp %s", input + 3);
        } else if (strncmp(input, "mv ", 3) == 0) {
            snprintf(command, sizeof(command), "dbxcli mv %s", input + 3);
        } else if (strncmp(input, "mkdir ", 6) == 0) {
            snprintf(command, sizeof(command), "dbxcli mkdir %s", input + 6);
        } else if (strncmp(input, "rm ", 3) == 0) {
            snprintf(command, sizeof(command), "dbxcli rm %s", input + 3);
        } else {
            printf("Comandă necunoscută!\n");
            print_help();
            continue;
        }

        // Execută comanda
        execute_command(command);
    }

    pthread_mutex_destroy(&log_mutex); // Eliberare mutex
    return 0;
}