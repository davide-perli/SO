#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define M 2 // nr linii din A
#define P 3 // nr coloane din A și de linii din B
#define N 2 // nr coloane din B

int A[M][P] = {{1, 2, 3}, {4, 5, 6}};
int B[P][N] = {{7, 8}, {9, 10}, {11, 12}};
int C[M][N] = {0}; // Matrice rezultat pe care o initializez cu 0

// Struct trimitere date la thread
typedef struct {
    int row;  // Indici i, j pentru linii/coloane din mat rezultat
    int col;
} ThreadData;

// Funcția rulată de thread pentru a calcula un element din matricea rezultat
void *calculate_element(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    int sum = 0;
    for (int k = 0; k < P; k++) {
        sum += A[data->row][k] * B[k][data->col]; // Formula de calculare a produsului elementelor din matrici
    }
    C[data->row][data->col] = sum; // Salvare date in matricea rezulat
    free(data); // Eliberare memorie thread dupa executie
    return NULL;
}

int main() {  // Matrice thread-uri pentru fiecare element
    pthread_t threads[M][N];  

    // Creez câte un thread pentru fiecare element din matricea rezultat
    for (int i = 0; i < M; i++) { // Rows matrix
        for (int j = 0; j < N; j++) { // Coloumns matrix
            ThreadData *data = malloc(sizeof(ThreadData)); // Alocare memorie dinamic pentru a stoca datele
            data->row = i;
            data->col = j;

            if (pthread_create(&threads[i][j], NULL, calculate_element, data)) { // Creeaza un nou thread si începe executia functiei calculate_element sau arunca o eroare
                perror("Eroare la crearea thread-ului");
                return 1;
            }
        }
    }

    // Dupa finalizarea tuturor thread-urilor afisez matricea rezultat
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            if (pthread_join(threads[i][j], NULL)) {
                perror("Eroare la așteptarea thread-ului");
                return 1;
            }
        }
    }

    // Afisare rezultat matriceal
    printf("Rezultatul produsului celor doua matrice:\n");
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", C[i][j]);
        }
        printf("\n");
    }

   /*
        Comenzi terminal : cc lab6_ex2.c -o lab6_ex2 -lpthread
                           ./lab6_ex2
    */

    return 0;
}
