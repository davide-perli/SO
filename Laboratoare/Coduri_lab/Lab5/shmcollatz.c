#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_NUMBERS 10 // maxim 10 numere
#define SHM_NAME "/myshm"
#define SHM_SIZE 1024 //martime 1024 bytes

void collatz_sequence(int num, char *output) {
    char buffer[100];
    sprintf(output, "%d: ", num);
    
    while (num != 1) {
        sprintf(buffer, "%d ", num);
        strcat(output, buffer);   // Concatenez la pointerul de output un buffer care indica indexul copilului * dimensiunea unui ccopil
        
        if (num % 2 == 0) {
            num /= 2;
        } else {
            num = 3 * num + 1;
        }
    }
    strcat(output, "1.");
}

int main(int argc, char *argv[]) {
    if (argc < 2) { //Verificare furnizare valori pentru testare algoritm
        fprintf(stderr, "Trebuie furnizate numere pentru testarea algoritmului Collatz\n");
        return EXIT_FAILURE;
    }

    // Creare memorie partajata
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (shm_fd < 0) {
        perror("shm_open");
        return EXIT_FAILURE;
    }

    // Configurare dimensiune
    if (ftruncate(shm_fd, SHM_SIZE) == -1) {
        perror("ftruncate");
        shm_unlink(SHM_NAME);
        return EXIT_FAILURE;
    }

    char *shm_ptr = mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm_ptr == MAP_FAILED) {
        perror("mmap");
        shm_unlink(SHM_NAME);
        return EXIT_FAILURE;
    }

    shm_ptr[0] = '\0'; // Memorie partajata initializare

    //Error handling daca primesc numere mai mici ca 1
    for (int i = 1; i < argc; i++) {
        int num = atoi(argv[i]);
        if (num < 1 || i > MAX_NUMBERS) {
            fprintf(stderr, "Introduceti numere >= 1 \n");
            munmap(shm_ptr, SHM_SIZE);
            shm_unlink(SHM_NAME);
            return EXIT_FAILURE;
        }

        // Proces Fork()
        pid_t pid = fork();
        if (pid == 0) {
            char output[SHM_SIZE / MAX_NUMBERS] = {0}; // Offset pentru fiecare copil
            collatz_sequence(num, output);
            strcat(shm_ptr, output); // Scriere in  memoria partajata
            exit(0);
        }
    }

    // Asteptare terminare procese
    for (int i = 1; i < argc; i++) {
        wait(NULL);
    }

    // Afisare rezultate
    printf("%s\n", shm_ptr);

    // shm unlink(3) s, i munmap(2) pentru a elibera resursele folosite.
    munmap(shm_ptr, SHM_SIZE);
    shm_unlink(SHM_NAME);
    
    return EXIT_SUCCESS;
}