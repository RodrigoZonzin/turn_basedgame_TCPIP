#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_LEN 1024
#define PORT 5005


int max(int a, int b){
    return (a>= b) ? a : b;
}

int main(){

    struct sockaddr_in servidor, cliente1, cliente2; 
    int servidorfd, cliente1fd, cliente2fd; 


    if(servidorfd = socket(AF_INET, SOCK_STREAM, 0) == -1){
        perror("Erro ao criar o socket\n");
        return EXIT_FAILURE;
    } else printf("Incializando o server!\n");

    
    servidor.sin_addr.s_addr = INADDR_ANY; 
    servidor.sin_port = htons(PORT);
    memset(servidor.sin_zero, 0x0, sizeof(servidor.sin_zero));

    //tratamento de erro para caso a porta ja esteja em uso
    int yes = 1;
    if(setsockopt(servidorfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1){
        perror("Porta ja esta em uso");
        return EXIT_FAILURE;
    }


    //dizendo pro SO que o socket esta em uso
    if(bind(servidorfd, (struct sockaddr*) &servidor, sizeof(servidor)) == -1){
        perror("Erro ao nomear socket");
        return EXIT_FAILURE;
    }
    else printf("Socket nomeado\n");

    //aguardar conexao dos clientes
    if(listen(servidorfd, 2) == -1){
        perror("Erro na escuta");
        return EXIT_FAILURE;
    }
    else printf("Escutando na porta: %d\n", PORT);

    //conectando o primeiro cliente
    socklen_t cliente1_len = sizeof(cliente1);
    if((cliente1fd = accept(servidorfd, (struct sockaddr*) &cliente1, &cliente1_len)) == -1){
        perror("Erro de conexao do primeiro jogador");
        return EXIT_FAILURE;
    }
    else printf("Primeiro jogador conectado com sucesso!\n");

    //conectando o segundo cliente
    socklen_t cliente2_len = sizeof(cliente2);
    if((cliente2fd = accept(servidorfd, (struct sockaddr*) &cliente2, &cliente2_len)) == -1){
        perror("Erro de conexao do segundo jogador");
        return EXIT_FAILURE;
    }
    else printf("Segundo jogador conectado com sucesso!\n");    
    printf("Jogadores conectados. Pronto para iniciar o jogo!\n");

    char buffer_in [BUF_LEN];  memset(buffer_in, 0x0, BUF_LEN);
    char buffer_out[BUF_LEN];  memset(buffer_out, 0x0, BUF_LEN);
    int jogada1, jogada2, resultado = 0;


    while(1){
        strcpy(buffer_out, "Jogue um numero e aguarde:\n");
        
        send(cliente1fd, buffer_out, strlen(buffer_out), 0);
        recv(cliente1fd, &jogada1, sizeof(int), 0);

        send(cliente2fd, buffer_out, strlen(buffer_out), 0);
        recv(cliente1fd, &jogada2, sizeof(int), 0);

        resultado = max(jogada1, jogada2);

        
        if(resultado == jogada1){
            strcpy(buffer_out, "Parabens! Voce venceu!\n");
            send(cliente1fd, buffer_out, sizeof(buffer_out), 0);

            strcpy(buffer_out, "Que pena! Voce perdeu!\n");
            send(cliente2fd, buffer_out, sizeof(buffer_out), 0);
        } else
        if(resultado == jogada2){
            strcpy(buffer_out, "Parabens! Voce venceu!\n");
            send(cliente2fd, buffer_out, sizeof(buffer_out), 0);

            strcpy(buffer_out, "Que pena! Voce perdeu!\n");
            send(cliente1fd, buffer_out, sizeof(buffer_out), 0);
        }

    }
    
    close(cliente1fd);
    close(cliente2fd);
    close(servidorfd);

    return 0;
}