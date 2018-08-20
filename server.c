#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
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
    if (argc < 2) {
        logexit("missing port param");
    }

    int s = socket(AF_INET, SOCK_STREAM, 0);

    if (s != -1) {
        struct in_addr addr = {
            .s_addr = htonl(INADDR_LOOPBACK)
        };

        struct sockaddr_in client, dst = {
            .sin_family = AF_INET,
            .sin_port = htons(atoi(argv[1])),
            .sin_addr = addr
        };

        struct sockaddr* sa_dst = (struct sockaddr*) &dst;

        if (bind(s, sa_dst, sizeof(dst)) >= 0) {
            int c;

            listen(s, 5);
            socklen_t client_length = sizeof(client);

            // a combinacao ctrl+c termina a execucao
            // cada iteracao do while representa uma nova conexao de cliente
            while ((c = accept(s, (struct sockaddr*) &client,  &client_length)) >= 0) {
                char buffer[BUFSZ];

                // define temporizador de 15 segundos
                struct timeval timeout;
                timeout.tv_sec = 15;
                timeout.tv_usec = 0;

                setsockopt(c, SOL_SOCKET, SO_RCVTIMEO, (char*) &timeout, sizeof(timeout));

                // tamanho da mensagem recebido
                if (recv(c, buffer, 4, MSG_WAITALL) == 4) {
                    uint32_t string_size = ntohl(*(uint32_t *)buffer);
                    printf("string_size: %d\n", string_size);

                    // recebe mensagem do cliente
                    if (recv(c, buffer, string_size, MSG_WAITALL) == string_size) {
                        snprintf(buffer, BUFSZ, "%s", buffer);
                        printf("message: %s\n", buffer);
                    //     int decoder_key;
                    //     snprintf(buffer, BUFSZ, "%s", buffer);

                    //     if (recv(c, &decoder_key, 1, MSG_WAITALL) == 1) {
                    //         printf("%d\n", decoder_key);
                    //     }

                    //     // valida confirmacao e persiste novo valor
                    //     // if (atoi(buffer) == new_value) {
                    //     //     counter = new_value;
                    //     //     printf("%d\n", counter);
                    //     // }
                    }
                } else {
                    // servidor temporizou
                    printf("T\n");
                }

                // fecha a conexao com o cliente
                close(c);
            }
        }
    }

    close(s);
    return 0;
}