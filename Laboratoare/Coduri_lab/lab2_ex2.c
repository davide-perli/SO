#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>

int main(int argc, char **argv) {

    const char *input = argv[1];
    const char *output = argv[2];

    int input = open(input, O_RDONLY);

    int output = open(output, O_WRONLY);

    char buffer[4096];
    ssize_t bytes_read;

    while ((bytes_read = read(input, buffer, sizeof(buffer))) > 0)
     {
        write(output, buffer, bytes_read);
     }

    close(input);
    close(output);

    return 0;
}