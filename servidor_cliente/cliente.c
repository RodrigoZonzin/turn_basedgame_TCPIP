#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 5002
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

    memset(server.sin_zero, 0x0, sizeof(server.sin_zero));
    //memset(server.sin_port, 0x0, sizeof(server.sin_port));
    //memset(server.sin_addr.s_addr, 0x0, sizeof(server.sin_addr.s_addr));
    //propriedades do server a se conectar
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY; //inet_addr(SERVER_ADDR);


    //TENTA SE CONECTAR AO SERVER
    if(connect(sockfd, (struct sockaddr*) &server, len) == -1){
        perror("Nao foi possivel se conectar ao server");
        return EXIT_FAILURE;
    }
    fprintf(stdout, "Conexao efetuada com sucesso\n");

    int autorizacao, saida, resposta;
    while(1){
        recv(sockfd, &autorizacao, sizeof(int), 0);
        if(autorizacao == 1){
            printf("Jogue um numero:\n");
        }

        //Receptando a jogada do usuario e transmitindo-a para o servidor
        scanf("%d", &saida); getchar();
        send(sockfd, &saida, sizeof(int), 0);


        //resposta ganhou ou perdeu
        recv(sockfd, &resposta, sizeof(int), 0); 
        if(resposta == 1){
            printf("Parabens! Voce ganhou!\n\n");
        }
        else if(resposta == 0){
            printf("Que pena! Voce Perdeu!\n\n");
        }

        //continuar jogando ou nao?
        printf("Deseja parar de jogar? Sim (1) ou Nao (0)\n");
        scanf("%d", &saida);
        
        //envia mensagem de continuar jogada ou nao
        send(sockfd, &saida, sizeof(int), 0);

        //recebe mensagem de desconexao ou nao de algum jogador
        recv(sockfd, &resposta, sizeof(int), 0);
        if(resposta == 1){
            printf("Jogador desistiu. Desconectando...\n");
            break;
        }


    }

    close(sockfd);
    fprintf(stdout, "\nConexao fechada\n\n");

    return EXIT_SUCCESS;
}