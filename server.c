#include <stdio.h>
#include <stdlib.h>
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
                char op;

                // define temporizador de 1 segundo
                struct timeval timeout;
                timeout.tv_sec = 1;
                timeout.tv_usec = 0;

                setsockopt(c, SOL_SOCKET, SO_RCVTIMEO, (char*) &timeout, sizeof(timeout));

                // mensagem recebida
                if (recv(c, &op, 1, MSG_WAITALL) == 1) {
                    char buffer[BUFSZ];
                    int new_value = counter;

                    // verifica o operador recebido e calcula o novo valor
                    if (op == '+') {
                        new_value += 1;
                    } else if (op == '-') {
                        new_value -= 1;
                    }

                    // calcula modulo do novo valor - somente valores entre 0 e 999
                    new_value = new_value % 1000;

                    if (new_value < 0) {
                        new_value += 1000;
                    }

                    // envia o novo valor para o cliente
                    uint32_t validator = htonl(new_value);
                    send(c, &validator, 4, 0);

                    // recebe confirmacao do cliente
                    if (recv(c, buffer, 3, MSG_WAITALL) == 3) {
                        snprintf(buffer, BUFSZ, "%3s", buffer);

                        // valida confirmacao e persiste novo valor
                        if (atoi(buffer) == new_value) {
                            counter = new_value;
                            printf("%d\n", counter);
                        }
                    }
                } else {
                    // servidor temporizou
                    printf("T\n");
                }

                close(c);
            }
        }
    }

    close(s);
    return 0;
}