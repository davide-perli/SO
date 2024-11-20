#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

#define INPUT_FOLDER "/home/davide/Downloads/tests01"
#define OUTPUT_FILE "output.txt"

int main() {
    DIR *dir;
    struct dirent *entry;
    struct stat file_stat;
    FILE *output;

    // Deschide directorul
    dir = opendir(INPUT_FOLDER);
    if (dir == NULL) {
        perror("Nu s-a putut deschide directorul");
        return EXIT_FAILURE;
    }

    // Deschide fisierul de iesire
    output = fopen(OUTPUT_FILE, "w");
    if (output == NULL) {
        perror("Nu s-a putut crea fisierul de iesire");
        closedir(dir);
        return EXIT_FAILURE;
    }

    // Parcurge fiecare fisier din director
    while ((entry = readdir(dir)) != NULL) {
        // Verifica daca extensia este .txt
        if (strstr(entry->d_name, ".txt") != NULL) {
            char filepath[512];
            snprintf(filepath, sizeof(filepath), "%s/%s", INPUT_FOLDER, entry->d_name);

            // Obtine informatiile despre fisier
            if (stat(filepath, &file_stat) == 0) {
                fprintf(output, "%s: %ld Bytes\n", entry->d_name, file_stat.st_size);
            } else {
                perror("Nu s-au putut obtine informatii despre fisier");
            }
        }
    }

    // Inchide directorul si fisierul
    closedir(dir);
    fclose(output);

    printf("Fisierele si dimensiunile lor au fost scrise în '%s'.\n", OUTPUT_FILE);
    return EXIT_SUCCESS;
}
