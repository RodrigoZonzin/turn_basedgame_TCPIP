#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 4002
#define BUF_LEN 4096


int main(int argc, char *argv[]){
    struct sockaddr_in server;
    int sockfd;

    int len = sizeof(server);
    int slen;

    char buffer_in[BUF_LEN];
    char buffer_out[BUF_LEN];

    fprintf(stdout, "Incializando...\n");
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("Erro na criacao do socket do cliente:");
        return EXIT_FAILURE;
    }
    //fprintf(stdout, "Cliente criado com socket: %d\n", sockfd);

    memset(server.sin_zero, 0x0, sizeof(server.sin_zero));
    //memset(server.sin_port, 0x0, sizeof(server.sin_port));
    //memset(server.sin_addr.s_addr, 0x0, sizeof(server.sin_addr.s_addr));
    //propriedades do server a se conectar
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY; //inet_addr(SERVER_ADDR);


    /* Tries to connect to the server */
    if (connect(sockfd, (struct sockaddr*) &server, len) == -1) {
        perror("Nao foi possivel se conectar ao server");
        return EXIT_FAILURE;
    }
    fprintf(stdout, "Conexao efetuada com sucesso\n");

    int *resposta;
    int saida;
    while (1) {
        //memset(buffer_in, 0x0, sizeof(buffer_in));
        //memset(buffer_out, 0x0, sizeof(buffer_out));
        
        //mensagem "Jogue um numero:\n"
        recv(sockfd, buffer_in, sizeof(buffer_in), 0);
        fprintf(stdout, "%s", buffer_in); 
        //memset(buffer_in, 0x0, sizeof(buffer_in));
        

        //Receptando a jogada do usuario e transmitindo-a para o servidor
        scanf("%d", &saida);
        send(sockfd, &saida, sizeof(int), 0);
        //memset(buffer_out, 0x0, sizeof(buffer_out));

        //resposta ganhou ou perdeu
        recv(sockfd, buffer_in, sizeof(buffer_in), 0); 
        fprintf(stdout, "%s\n", buffer_in); 
        memset(buffer_in, 0x0, sizeof(buffer_in));
    }

    close(sockfd);
    fprintf(stdout, "\nConexao fechada\n\n");

    return EXIT_SUCCESS;
}