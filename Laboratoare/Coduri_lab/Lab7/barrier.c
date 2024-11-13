#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>

#define NTHRS 5 // Nr fire de executie

pthread_mutex_t mtx;
sem_t sem;
int threads_at_barrier = 0; // Numar cate fire au ajuns la bariera

// Initializez bariera
void init_barrier(int n) {
    pthread_mutex_init(&mtx, NULL); // Initializare mutex
    sem_init(&sem, 0, 0); // Initializare semafor cu 0
}

// Functia care implementeaza punctul de bariera
void barrier_point() {
    pthread_mutex_lock(&mtx); // Block mutex pentru a actualiza contorul

    threads_at_barrier++; // Incrementez numarul de fire ajunse la bariera

    if (threads_at_barrier == NTHRS) { // Verific daca toate firele au ajuns
        for (int i = 0; i < NTHRS; i++) {
            sem_post(&sem); // Permit tuturor firelor sa continue (Incrementeaza contorul S)
        }
    }

    pthread_mutex_unlock(&mtx); // Unblock mutex
    sem_wait(&sem); // Fiecare fir asteapta la bariera pana cand este eliberat (Decrementeaza contorul S)
}

// Functia executata de fiecare thread
void* tfun(void* v) {
    int tid = *(int*)v; // ID-ul thread-ului
    printf("%d reached the barrier\n", tid);

    barrier_point(); // Firul ajunge la punctul de bariera (asteapta ca fiecare thread sa ajunga la bariera)

    printf("%d passed the barrier\n", tid);
    free(v);
    return NULL;
}

int main() {
    pthread_t threads[NTHRS];
    init_barrier(NTHRS); // Initializare bariera

    for (int i = 0; i < NTHRS; i++) {
        int* tid = malloc(sizeof(int));
        *tid = i;
        pthread_create(&threads[i], NULL, tfun, tid);
    }

    for (int i = 0; i < NTHRS; i++) {
        pthread_join(threads[i], NULL); // Se asteapta terminarea fiecarui thread
    }

    pthread_mutex_destroy(&mtx); // Distrugere mutex la final
    sem_destroy(&sem); // Distrugere semafor la final
    return 0;
}
