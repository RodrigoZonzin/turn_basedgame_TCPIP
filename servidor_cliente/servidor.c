#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>

#define PORT 5002
#define BUF_LEN 4096

#define DEBUGA printf("CU CU CU CU\n"); 

int max(int a, int b){
    if(a > b) return a;
    else return b;
}

int main() {
    struct sockaddr_in server, client1, client2;
    int serverfd, client1fd, client2fd = -1; // Inicialize clientfd

    char buffer[BUF_LEN];

    fprintf(stdout, "Inicializando o server\n");

    //criando o socket do server
    serverfd = socket(AF_INET, SOCK_STREAM, 0);

    if(serverfd == -1){
        perror("Nao foi possivel criar o socket: ");
        return EXIT_FAILURE;
    }
    else fprintf(stdout, "Server criado com sucesso!\n");

    //definindo as propriedades do servidor 
    server.sin_family = AF_INET;            //para IPv4
    server.sin_port = htons(PORT);          //setando a porta
    server.sin_addr.s_addr = INADDR_ANY;    //qualquer ip

    //tratamento de erro para caso a porta ja esteja em uso
    int yes = 1;
    if(setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1){
        perror("Porta ja esta em uso");
        return EXIT_FAILURE;
    }

    //dizendo pro SO que o socket esta em uso
    if(bind(serverfd, (struct sockaddr*) &server, sizeof(server)) == -1){
        perror("Erro ao nomear socket");
        return EXIT_FAILURE;
    }

    //aguardar conexao dos clientes
    if(listen(serverfd, 2) == -1){
        perror("Erro na escuta");
        return EXIT_FAILURE;
    }
    fprintf(stdout, "Escutando na porta: %d\n", PORT);

    //conectando o primeiro cliente
    socklen_t client1_len = sizeof(client1);
    if((client1fd = accept(serverfd, (struct sockaddr*) &client1, &client1_len)) == -1){
        perror("Erro de conexao do primeiro jogador");
        return EXIT_FAILURE;
    }

    //conectando o segundo cliente
    socklen_t client2_len = sizeof(client2);
    if((client2fd = accept(serverfd, (struct sockaddr*) &client2, &client2_len)) == -1){
        perror("Erro de conexao do segundo jogador");
        return EXIT_FAILURE;
    }
    //registro de boas vindas
    fprintf(stdout, "Os jogadores foram conectados com sucesso\n");

    char mensagem_jogada[100];

    while(1){
        int jogadaDe1, jogadaDe2, resultado = 0; 
        strcpy(mensagem_jogada, "Jogue um numero: \n");

        send(client1fd, mensagem_jogada, strlen(mensagem_jogada), 0);
        if(recv(client1fd, buffer, sizeof(buffer), 0) <= 0){
            perror("Erro ao receber jogada do primeiro jogador"); 
            return EXIT_FAILURE;
        }
        sscanf(buffer, "%d", &jogadaDe1); 

        send(client2fd, mensagem_jogada, strlen(mensagem_jogada), 0);
        if(recv(client2fd, buffer, sizeof(buffer), 0) <= 0){
            perror("Erro ao receber jogada do primeiro jogador"); 
            return EXIT_FAILURE;
        }
        sscanf(buffer, "%d", &jogadaDe2); 

        resultado = max(jogadaDe1, jogadaDe2);

        if(resultado == jogadaDe1){
            printf("Jogador 1 ganhou!\n");
            strcpy(mensagem_jogada, "Parabens! Voce venceu!\n\n");
            send(client1fd, mensagem_jogada, strlen(mensagem_jogada), 0); 
            
            strcpy(mensagem_jogada, "Que pena! Voce perdeu!\n\n");
            send(client2fd, mensagem_jogada, strlen(mensagem_jogada), 0); 
        }

        else if(resultado == jogadaDe2){
            printf("Jogador 2 ganhou!\n");
            strcpy(mensagem_jogada, "Parabens! Voce venceu!\n\n");
            send(client2fd, mensagem_jogada, strlen(mensagem_jogada), 0); 
            
            strcpy(mensagem_jogada, "Que pena! Voce perdeu!\n\n");
            send(client1fd, mensagem_jogada, strlen(mensagem_jogada), 0); 
        }

        strcpy(buffer, "Deseja continuar?\n"); 
        
        /*char continuar1, continuar2; 
        send(client1fd, buffer, sizeof(buffer), 0);
        recv(client1fd, &continuar1, sizeof(char), 0);

        send(client2fd, buffer, sizeof(buffer), 0);
        recv(client2fd, &continuar2, sizeof(char), 0);

        char mensagem_desconexao[] = "A partida se encerrara por desconexao de um jogador. Obrigado por jogar!\n"; 
        if(continuar1 == 'n' && continuar2 == 'n'){
            //strcpy(mensagem_jogada, );
            send(client1fd, mensagem_jogada, strlen(mensagem_desconexao), 0);
            send(client2fd, mensagem_jogada, strlen(mensagem_desconexao), 0);
            break;
        }
        DEBUGA
        */

    }


    close(client1fd);
    close(client2fd);
    close(serverfd);

    printf("Conexao fechada!\n\n");


    return 0; 
}