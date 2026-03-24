#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

char *separator = " -\r\t\n./,";

void process_number(char *num) {
   int n = atoi(num);
   if (n %5 == 0 || n % 6 == 0) {
       printf("%d\n", n);
   }
}

void sixfive(int fd) {
    char c;
    char buf[512];
    int i = 0;
    while (read(fd, &c, 1) > 0) {
        
        if (c >= '0' && c <= '9') {
            buf[i++] = c;
        } 
        else if (strchr(separator, c) != 0) {
            if (i > 0) {
                buf[i] = '\0';
                process_number(buf);
                i = 0;
            }
        }
        else {
            i = 0;
        }

        if (i > 0) {
            buf[i] = '\0';
            process_number(buf);
        }
    }
}

int main(int argc, char *argv[]) {
    int fd;

    if (argc <= 1) {
        fprintf(2, "please input file name.");
        exit(1);
    }
    for (int i = 1; i < argc; i++) {
        if ((fd = open(argv[i], 0)) < 0) {
            fprintf(2, "sixfive: cannot open %s\n", argv[i]);
            exit(1);
        }
        sixfive(fd);
        close(fd);
    }

    exit(0);
}