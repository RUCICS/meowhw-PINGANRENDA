#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    char buf[1];
    ssize_t n;
    
    while ((n = read(fd, buf, sizeof(buf))) > 0) {
        if (write(STDOUT_FILENO, buf, n) != n) {
            perror("write");
            exit(EXIT_FAILURE);
        }
    }

    if (n == -1) {
        perror("read");
        exit(EXIT_FAILURE);
    }

    close(fd);
    return EXIT_SUCCESS;
}