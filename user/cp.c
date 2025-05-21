#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

#define BUF_SIZE 512

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(2, "Usage: cp <source> <destination>\n");
        exit(1);
    }

    int src = open(argv[1], O_RDONLY);
    if (src < 0) {
        fprintf(2, "cp: cannot open source file %s\n", argv[1]);
        exit(1);
    }

    int dst = open(argv[2], O_CREATE | O_WRONLY);
    if (dst < 0) {
        fprintf(2, "cp: cannot create destination file %s\n", argv[2]);
        close(src);
        exit(1);
    }

    char buf[BUF_SIZE];
    int n;
    while ((n = read(src, buf, sizeof(buf))) > 0) {
        if (write(dst, buf, n) != n) {
            fprintf(2, "cp: write error to %s\n", argv[2]);
            close(src);
            close(dst);
            exit(1);
        }
    }

    if (n < 0) {
        fprintf(2, "cp: read error from %s\n", argv[1]);
    }

    close(src);
    close(dst);
    exit(0);
}
