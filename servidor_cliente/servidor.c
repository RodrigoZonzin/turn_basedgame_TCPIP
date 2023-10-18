#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>

#define PORT 5002
#define BUF_LEN 4096

#define DEBUGA printf("CU CU CU CU\n"); 

int contador = 0;

int max(int a, int b){
    return (a >= b) ? a : b; 
}

int main(){
    struct sockaddr_in server, client1, client2;
    int serverfd, client1fd, client2fd = -1; 

    printf("Inicializando o server\n");

    //CRIANDO O DESCRITOR DE ARQUIVOS PARA O SOCKET DO SERVER
    serverfd = socket(AF_INET, SOCK_STREAM, 0);
    if(serverfd == -1){
        perror("Nao foi possivel criar o socket: ");
        return EXIT_FAILURE;
    }
    else printf("Server criado com sucesso!\n");

    //DEFINDINDO AS PROPRIEDADES DO SERVER
    server.sin_family = AF_INET;            //para IPv4
    server.sin_port = htons(PORT);          //setando a porta
    server.sin_addr.s_addr = INADDR_ANY;    //qualquer ip

    //CASO A PORTA JA ESTEJA EM USO
    int yes = 1;
    if(setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1){
        perror("Porta ja esta em uso");
        return EXIT_FAILURE;
    }

    //NOMENADO SOCKET NO SISTEMA OPERACIONAL
    if(bind(serverfd, (struct sockaddr*) &server, sizeof(server)) == -1){
        perror("Erro ao nomear socket");
        return EXIT_FAILURE;
    }

    //AGUARDANDO CONEXAO DOS CLIENTES
    if(listen(serverfd, 2) == -1){
        perror("Erro na escuta");
        return EXIT_FAILURE;
    }
    printf("Escutando na porta: %d\n", PORT);

    //CONECTANDO PRIMEIRO CLIENTE
    socklen_t client1_len = sizeof(client1);
    if((client1fd = accept(serverfd, (struct sockaddr*) &client1, &client1_len)) == -1){
        perror("Erro de conexao do primeiro jogador");
        return EXIT_FAILURE;
    }
    printf("Primeiro jogador conectado com sucesso!\n");

    //CONECTANDO SEGUNDO CLIENTE
    socklen_t client2_len = sizeof(client2);
    if((client2fd = accept(serverfd, (struct sockaddr*) &client2, &client2_len)) == -1){
        perror("Erro de conexao do segundo jogador");
        return EXIT_FAILURE;
    }
    fprintf(stdout, "Segundo jogador conectado com sucesso!\n");
    fprintf(stdout, "Prontos para comecar!\n\n");

    char buffer_in [BUF_LEN];  memset(buffer_in, 0x0, BUF_LEN);
    char buffer_out[BUF_LEN];  memset(buffer_out, 0x0, BUF_LEN);
    int jogadaDe1, jogadaDe2, resultado = 0;

    while(1){
        jogadaDe1, jogadaDe2, resultado = 0; 
        memset(buffer_in, 0x0, sizeof(buffer_in));
        memset(buffer_out, 0x0, sizeof(buffer_out));

        /*COMANDO DE JOGADA*/
        strcpy(buffer_out, "Jogue um numero:\n");               
        
        //ENVIANDO PARA O PRIMEIRO JOGADOR
        send(client1fd, buffer_out, strlen(buffer_out), 0);        
        if(recv(client1fd, &jogadaDe1, sizeof(int), 0) <= 0){
            perror("Erro ao receber jogada do primeiro jogador"); 
            return EXIT_FAILURE;
        }
        printf("Jogada de cliente 1: %d\n", jogadaDe1);

        //ENVIANDO PARA O SEGUNDO JOGADOR
        send(client2fd, buffer_out, strlen(buffer_out), 0);
        if(recv(client2fd, &jogadaDe2, sizeof(int), 0) <= 0){
            perror("Erro ao receber jogada do segundo jogador"); 
            return EXIT_FAILURE;
        }
        printf("Jogada de cliente 2: %d\n", jogadaDe2);

        //COMPUTANDO O RESULTADO, QUE É A MAIOR JOGADA DE ALGUM DOS DOIS JOGADORES
        resultado = max(jogadaDe1, jogadaDe2);

        //CASO O RESULTADO SEJA IGUAL A JOGADA DO JOGADOR 1
        //ENVIAMOS PARA ELE A MENSAGEM DE PARABENIZACAO E
        //ENVIAMOS PARA O JOGADOR 2 A MENSAGEM DE PESAR
        if(resultado == jogadaDe1){
            printf("Jogador 1 ganhou!\n\n");
            strcpy(buffer_out, "Parabens! Voce venceu!\n");
            send(client1fd, buffer_out, strlen(buffer_out), 0); 
            memset(buffer_out, 0x0, sizeof(buffer_out));
            
            strcpy(buffer_out, "Que pena! Voce perdeu!\n");
            send(client2fd, buffer_out, strlen(buffer_out), 0); 
            memset(buffer_out, 0x0, sizeof(buffer_out));
        }

        if(resultado == jogadaDe2){
            printf("Jogador 2 ganhou!\n\n");
            strcpy(buffer_out, "Parabens! Voce venceu!\n");
            send(client2fd, buffer_out, strlen(buffer_out), 0); 
            memset(buffer_out, 0x0, sizeof(buffer_out));
            
            strcpy(buffer_out, "Que pena! Voce perdeu!\n");
            send(client1fd, buffer_out, strlen(buffer_out), 0); 
            memset(buffer_out, 0x0, sizeof(buffer_out));
        }

    }


    close(client1fd);
    close(client2fd);
    close(serverfd);

    printf("Conexao fechada!\n\n");


    return 0; 
}