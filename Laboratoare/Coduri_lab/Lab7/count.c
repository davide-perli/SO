#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>

#define MAX_RESOURCES 5

int available_resources = MAX_RESOURCES; // Numarul total de resurse disponibile
pthread_mutex_t mtx; // Mutex-ul pentru a controla accesul la resurse

// Functia de decrementare a resurselor
int decrease_count(int count) {
    pthread_mutex_lock(&mtx); // Block mutex pentru a intra în zona critica

    if (available_resources < count) { // Verific pentru resurse suficiente
        pthread_mutex_unlock(&mtx); // Unblock mutex si return error
        return -1;
    }
    
    available_resources -= count; // Scad resursele
    printf("Got %d resources %d remaining\n", count, available_resources);

    pthread_mutex_unlock(&mtx); // Unblock mutex la finalul modificarii
    return 0;
}

// Functia de incrementare a resurselor
int increase_count(int count) {
    pthread_mutex_lock(&mtx); // Block mutex pentru a intra în zona critica

    available_resources += count; // Cresc resursele
    printf("Released %d resources %d remaining\n", count, available_resources);

    pthread_mutex_unlock(&mtx); // Unblock mutex la finalul modificarii
}

// Functia executata de fiecare thread
void* thread_routine(void* arg) {
    int count = *((int*)arg); // Nr resurse cerute de thread

    if (decrease_count(count) == 0) { // Incearc sa obtina resursele
        // Simulez utilizarea resurselor
        increase_count(count); // Returnez resursele
    } else {
        printf("Not enough resources for %d resources request\n", count);
    }
    return NULL;
}

int main() {
    pthread_mutex_init(&mtx, NULL); // Initializare mutex
    pthread_t threads[5];
    int resources_needed[5] = {2, 2, 1, 3, 2}; // Resursele cerute de fiecare thread

    for (int i = 0; i < 5; i++) {
        pthread_create(&threads[i], NULL, thread_routine, &resources_needed[i]);
    }

    for (int i = 0; i < 5; i++) {
        pthread_join(threads[i], NULL); // Se asteapta terminarea fiecarui thread
    }

    pthread_mutex_destroy(&mtx); // Distrugere mutex la final
    return 0;
}
