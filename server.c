#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <arpa/inet.h>

#define BUFSZ 64

int main(int argc, char **argv) {
    int counter = 0;
    int s = socket(AF_INET, SOCK_STREAM, 0);

    if (s != -1) {
        struct in_addr addr = {
            .s_addr = htonl(INADDR_LOOPBACK)
        };

        struct sockaddr_in client, dst = {
            .sin_family = AF_INET,
            .sin_port = htons(51515),
            .sin_addr = addr
        };

        struct sockaddr* sa_dst = (struct sockaddr*) &dst;

        if (bind(s, sa_dst, sizeof(dst)) >= 0) {
            int c;

            listen(s, 5);
            socklen_t client_length = sizeof(client);

            // cada iteracao do while representa uma nova conexao de cliente
            while ((c = accept(s, (struct sockaddr*) &client,  &client_length)) >= 0) {
                char op, buffer[BUFSZ];

                // mensagem recebida
                if (recv(c, &op, 1, MSG_WAITALL) == 1) {
                    int new_value = 0;

                    // verifica o operador recebido e calcula o novo valor
                    if (op == '+') {
                        new_value = counter + 1;
                    } else if (op == '-') {
                        new_value = counter - 1;
                    }

                    // calcula modulo do novo valor - somente valores entre 0 e 999
                    new_value = new_value % 1000;

                    if (new_value < 0) {
                        new_value += 1000;
                    }

                    uint32_t validator = htonl(new_value);
                    snprintf(buffer, BUFSZ, "%03d", validator);

                    send(c, buffer, 4, 0);
//
//                    if (recv(c, buffer, 4, MSG_WAITALL) == 4) {
//                        printf("recebeu");
//                        uint32_t valid = ntohl(*(uint32_t *)buffer);
//
//                        if (valid == new_value) {
//                            counter = new_value;
//                            printf("%d\n", counter);
//                        }
//
//                        close(c);
//                    }
                }
            }
        }
    }

    close(s);
    return 0;
}