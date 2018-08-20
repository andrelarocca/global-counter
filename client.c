#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <arpa/inet.h>

#define BUFSZ 64

void logexit (const char *str) {
    if (errno) {
        perror(str);
    } else {
        puts(str);
    }

    exit(EXIT_FAILURE);
}

int main (int argc, char **argv) {
    if (argc < 5) {
        logexit("missing params");
    }

    int s = socket(AF_INET, SOCK_STREAM, 0);

    if (s == -1) {
        logexit("socket");
    }

    struct in_addr addr;
    inet_aton(argv[1], &addr);

    struct sockaddr_in dst = {
        .sin_family = AF_INET,
        .sin_port = htons(atoi(argv[2])),
        .sin_addr = addr
    };

    struct sockaddr *sa_dst = (struct sockaddr *)&dst;

    if (connect(s, sa_dst, sizeof(dst))) {
        logexit("connect");
    }

    uint32_t string_size = htonl(atoi(argv[4]));
    send(s, &string_size, 4, 0);
    // send(s, argv[3], strlen(argv[3]), 0);
    // char buf[BUFSZ];

    // if (recv(s, buf, 4, MSG_WAITALL) != 4) {
    //     logexit("recv\n");
    // }

    // uint32_t counter = ntohl(*(uint32_t *)buf);
    // snprintf(buf, BUFSZ, "%03d", counter);
    // send(s, buf, 3, 0);
    // printf("%d\n", counter);

    close(s);
    exit(EXIT_SUCCESS);
}
