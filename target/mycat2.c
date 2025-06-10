#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

size_t io_blocksize() {
    long sz = sysconf(_SC_PAGESIZE);
    if (sz <= 0) return 4096; // fallback
    return (size_t)sz;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file>\n", argv[0]);
        return 1;
    }
    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        perror("open");
        return 1;
    }
    size_t bufsize = io_blocksize();
    char *buf = (char*)malloc(bufsize);
    if (!buf) {
        perror("malloc");
        close(fd);
        return 1;
    }
    ssize_t n;
    while ((n = read(fd, buf, bufsize)) > 0) {
        ssize_t written = 0;
        while (written < n) {
            ssize_t w = write(STDOUT_FILENO, buf + written, n - written);
            if (w < 0) {
                perror("write");
                free(buf);
                close(fd);
                return 1;
            }
            written += w;
        }
    }
    if (n < 0) perror("read");
    free(buf);
    close(fd);
    return n < 0 ? 1 : 0;
}