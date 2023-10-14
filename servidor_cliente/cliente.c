#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/* Defines the server port */
#define PORT 5002

/* Sockets buffers length */
#define LEN 4096

/* Server address */
#define SERVER_ADDR "127.0.0.1"


int main(int argc, char *argv[]){

    struct sockaddr_in server;
    int sockfd;

    int len = sizeof(server);
    int slen;

    char buffer_in[LEN];
    char buffer_out[LEN];


    fprintf(stdout, "Inicializando cliente ...\n");
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("Erro na criacoa do socket do cliente:");
        return EXIT_FAILURE;
    }
    fprintf(stdout, "Cliente criado com socket: %d\n", sockfd);


    //propriedades do server a se conectar
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY; //inet_addr(SERVER_ADDR);
    memset(server.sin_zero, 0x0, 8);


    /* Tries to connect to the server */
    if (connect(sockfd, (struct sockaddr*) &server, len) == -1) {
        perror("Nao foi possivel se conectar ao server");
        return EXIT_FAILURE;
    }
    fprintf(stdout, "Conexao efetuada com sucesso\n");

    char input_valor[1];
    while(1){
        memset(buffer_in, 0x0, sizeof(buffer_in));
        recv(sockfd, buffer_in, sizeof(buffer_in), 0); 
        fprintf(stdout, "%s\n", buffer_in); 
        fprintf(stdout, ">>");
        scanf("%c", input_valor); 

        send(sockfd, input_valor, sizeof(input_valor), 0);


    }

    close(sockfd);
    fprintf(stdout, "\nConexao fechada\n\n");

    return EXIT_SUCCESS;
}