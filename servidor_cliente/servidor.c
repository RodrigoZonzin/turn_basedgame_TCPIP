#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>

#define PORT 5002
#define BUF_LEN 4096

#define DEBUGA printf("CU CU CU CU\n"); 


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

    int comandoJogada; 
    int jogadaDe1, jogadaDe2, resultado = 0;
    int respostaJogador1, respostaJogador2 = -1; 

    while(1){
        jogadaDe1 = jogadaDe2 = resultado = 0; 
        /*COMANDO DE JOGADA*/
        comandoJogada = 1;              

        //ENVIANDO PARA O PRIMEIRO JOGADOR
        send(client1fd, &comandoJogada, sizeof(int), 0);        
        if(recv(client1fd, &jogadaDe1, sizeof(int), 0) <= 0){
            perror("Erro ao receber jogada do primeiro jogador"); 
            return EXIT_FAILURE;
        }
        printf("Jogada de cliente 1: %d\n", jogadaDe1);

        //ENVIANDO PARA O SEGUNDO JOGADOR
        send(client2fd, &comandoJogada, sizeof(int), 0);
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
            respostaJogador1 = 1, respostaJogador2 = 0;
            printf("Jogador 1 ganhou!\n\n");

            send(client1fd, &respostaJogador1, sizeof(int), 0);             
            send(client2fd, &respostaJogador2, sizeof(int), 0); 
        }

        if(resultado == jogadaDe2){
            respostaJogador1 = 0, respostaJogador2 = 1;
            printf("Jogador 2 ganhou!\n\n");
        
            send(client1fd, &respostaJogador1, sizeof(int), 0);             
            send(client2fd, &respostaJogador2, sizeof(int), 0); 
        }

        //recebe mensagem de continuar jogada ou nao
        recv(client1fd, &jogadaDe1, sizeof(int), 0);
        recv(client2fd, &jogadaDe2, sizeof(int), 0);

        //envia mensagem de desconexao
        if((jogadaDe1 == 1) || (jogadaDe2 == 1)){
            respostaJogador1 = respostaJogador2 = 1;
            send(client1fd, &respostaJogador1, sizeof(int), 0);
            send(client2fd, &respostaJogador2, sizeof(int), 0);
            break;
        }
        else{
            respostaJogador1 = respostaJogador2 = 0;
            send(client1fd, &respostaJogador1, sizeof(int), 0);
            send(client2fd, &respostaJogador2, sizeof(int), 0);
        }

    }


    close(client1fd);
    close(client2fd);
    close(serverfd);

    printf("Conexao fechada!\n\n");


    return 0; 
}