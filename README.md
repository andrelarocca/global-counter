# Universidade Federal de Minas Gerais
## DCC023: Redes de Computadores
### Trabalho Prático

* Introdução
* Objetivos
* Execução
* Especificação
* Programa Cliente
* Programa Servidor
* Detalhes de Implementação
* Entrega e Avaliação

## Introdução
Neste trabalho iremos desenvolver um servidor que gerencia um contador global modificado por varios clientes. No texto abaixo, nomes de funções da biblioteca padrão relativas à tarefa sendo descrita são colocados entre colchetes para facilitar o desenvolvimento do trabalho.
## Objetivos
* Introduzir a interface de programação de soquetes POSIX.
* Introduzir os conceitos de aplicação cliente e aplicação servidor.
* Introduzir os conceitos de codificação e transmissão de dados.
## Execução
* O trabalho é individual e vale 5 pontos.
* A data de entrega está disponível no Moodle ou no Plano de Curso.
## Especificação
### Programa Cliente
O programa cliente se conecta ao servidor [socket, bind, connect]. Após estabelecimento da conexão, o cliente envia um (1) byte contendo os caracteres “+” ou “-”, requisitando o incremento ou decremento do contador global (módulo 1000, de forma que o contador só possa assumir valores entre 0 e 999) [send]. Após o envio da requisição, o cliente espera do servidor um inteiro de 4 bytes codificado em network byte order [htonl, ntohl] contendo o valor do contador global [recv]. Após recebimento do contador global, o cliente reenvia o valor recebido para o servidor em três caracteres numéricos (em formato ASCII) [sprintf], primeiro o caractere representando as centenas, depois dezenas e finalmente as unidades, confirmando o recebimento e processamento da mensagem [send]. Após o envio da comunicação, o cliente fecha a conexão com o servidor [close].
### Programa Servidor
O programa servidor inicializará seu contador com zero e irá esperar a conexão de clientes [socket, bind, listen, accept]. Após o estabelecimento de uma conexão, o servidor deverá receber um (1) byte de requisição [recv]. Após o recebimento da requisição, o servidor deverá processá-la e enviar o próximo valor do contador global para o cliente [send]; o valor do contador deve ser enviado como um inteiro de 4 bytes codificado em network byte order [htonl, ntohl]. Após o envio do próximo valor do contador, o servidor deverá esperar três bytes contendo a confirmação do valor do contador pelo cliente [recv]. Após o recebimento da confirmação (ou disparo do temporizador), o servidor deve fechar a conexão com o cliente [close]. Se o valor confirmado pelo cliente for o próximo valor do contador [atoi], o servidor atualiza o contador global e escreve o novo valor em uma nova linha na saída padrão. Se o servidor temporizar, ele deve escrever “T” em uma nova linha. O servidor deve então voltar ao estágio inicial para aceitar a próxima conexão de cliente (mantendo o valor do contador). Para facilitar a correção, seu servidor não deve escrever nada além do indicado anteriormente. Isto é, se der algum erro que não seja a temporização, como se o valor retornado não for o valor esperado, etc., o servidor não deve escrever nada na saída.
## Detalhes de Implementação
* O servidor deverá escutar no endereço IP 127.0.0.1 (representando a máquina local em que o programa executa) e no porto 51515.
* Os caracteres '+' e '-' devem ser transmitidos em 1 byte usando codificação ASCII.
* O servidor deve configurar um temporizador (timeout) para detectar falhas de comunicação ao chamar a função [recv]. A configuração de temporizador é feita chamando-se a função [setsockopt]. No Linux, as opções disponíveis para uso na função [setsockopt] são descritas na seção [socket] do manual 7 (acesse usando [man 7 socket]). Procure por [SO_RCVTIMEO]. Em python usa-se o método [settimeout] e usa-se o tratamento da exceção [socket.timeout]. O temporizador deve ser configurado com o tempo de 1 segundo.
* O servidor deve imprimir o novo valor do contador na saída padrão (stdout) toda vez que o contador for modificado com sucesso.
* O servidor deve executar indefinidamente, até ser terminado pelo usuário. Por exemplo, o servidor pode terminar de executar quando o usuário apertar uma tecla específica, apertar CTRL+C, ou enviar um sinal de terminação para o processo.
* Em Python, ao invés de usar htonl/ntohl, deve-ser usar as funções [pack] e [unpack].
## Entrega e Avaliação
Você deve entregar o código fonte do seu programa e um makefile com o alvo "run", de forma que seu programa será iniciado ao se executar "make run", e um alvo vazio para compilar seu programa se você usar C (quer dizer, executar "make" compila e gera o executável). Para este trabalho não é preciso entregar um relatório, mas seu código deve estar bem organizado e comentado. Seu programa será testado semi-automaticamente com a implementação de referência do professor. Para testar o correto funcionamento do seu servidor, teste seu programa com o cliente de referência disponível no moodle.
