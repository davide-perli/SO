#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

// Struct trimitere/primire date la/de la thread
typedef struct {
    char *input;
    char *output;
} ThreadData;

// Funcția de thread pentru inversarea sirului
void *reverse_string(void *arg) {
    ThreadData *data = (ThreadData *)arg;  // Pointern catre ThreadData care e struct
    int len = strlen(data->input); // len(sir intrare)

    for (int i = 0; i < len; i++) {
        data->output[len - i - 1] = data->input[i]; //parcurgere pe dos pentru inversare
    }

    return NULL;
}

// Error handling  daca nu primesc argumente (niciun caracter)
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Introduceti minim un caracter pentru ca programul sa afiseze in consola inversul\n");
        return 1;
    }

    pthread_t thread;
    ThreadData data; // Contine campurile input si output ca cin si cout din c++
    data.input = argv[1]; // Initializare input cu sirul primit cand dau comanda in terminal
    data.output = malloc(strlen(data.input) + 1); // Alocare memorie dinamic pentru a stoca o copie 

    if (pthread_create(&thread, NULL, reverse_string, &data)) {  // Functia de inversare in alt thread diferit
        perror("Eroare la crearea thread-ului");
        return 1;
    }

    if (pthread_join(thread, NULL)) {   // Dupa finalizarea thread-ului afisez sirul inversat
        perror("Eroare la finalizarea thread-ului");
        return 1;
    }

    printf("Șir inversat: %s\n", data.output);  //Afisare rezultat
    free(data.output); // Eliberare memorie la finalul executiei

    return 0;

    /*
        Comenzi terminal : cc lab6_ex1.c -o lab6_ex1 -lpthread
                           ./lab6_ex1 tulas 
                           ./lab6_ex1
    */
}
