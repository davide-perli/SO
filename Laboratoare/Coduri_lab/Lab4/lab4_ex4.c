#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>

void collatz(int n) {
    printf("%d: ", n);
    while (n != 1) {
        printf("%d ", n);
        if (n % 2 == 0) {
            n /= 2;
        } else {
            n = 3 * n + 1;
        }
    }
    printf("1.\n");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <number1> <number2> ...\n", argv[0]);
        return 1;
    }

    pid_t pids[argc - 1];
    printf("Starting parent %d\n", getpid());

    for (int i = 1; i < argc; i++) {
        int number = atoi(argv[i]);
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork failed");
            return errno;
        } else if (pid == 0) {
            // Child process
            if(number < 1)
            {
                perror("stop"); 
            }
            else{
            collatz(number);
            exit(0);
            }
        } else {
            pids[i - 1] = pid;
        }
    }

    for (int i = 0; i < argc - 1; i++) {
        waitpid(pids[i], NULL, 0);
        printf("Done Parent %d Me %d\n", getpid(), pids[i]);
    }

    return 0;
}
