#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(2, "Usage: touch <filename>\n");
        exit(1);
    }


    int fd = open(argv[1], O_RDONLY);
    if (fd >= 0) {

        close(fd);
        fprintf(2, "touch: file '%s' already exists\n", argv[1]);
        exit(1);
    }

    fd = open(argv[1], O_CREATE | O_WRONLY);
    if (fd < 0) {
        fprintf(2, "touch: failed to create file '%s'\n", argv[1]);
        exit(1);
    }

    close(fd);
    exit(0);
}
