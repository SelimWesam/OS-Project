#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(2, "Usage: mv <source> <destination>\n");
        exit(1);
    }

    // link = create new name (hard link)
    if (link(argv[1], argv[2]) < 0) {
        fprintf(2, "mv: cannot link %s to %s\n", argv[1], argv[2]);
        exit(1);
    }

    // unlink = remove old name
    if (unlink(argv[1]) < 0) {
        fprintf(2, "mv: warning: linked but cannot remove original file %s\n", argv[1]);
        exit(1);
    }

    exit(0);
}
