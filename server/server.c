#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>

#include "odd_even.h"

int main(){

    struct sockaddr_in client_addr; 
    struct sockaddr_in server_addr = {
        .sin_family         = AF_INET,               /*cause we're using IPv4 adressess*/
        .sin_addr.s_addr    = htonl(INADDR_ANY),
        .sin_port           = htons(5000)            /*port where the server will be listening*/
    };

    int server = socket(AF_INET, SOCK_STREAM, 0); 
    //SOCK_STREAM ==> TCP 
    // SOCK_DGRAM  ==> UDP
        
    bind(server, (struct sockaddr *) &server_addr, sizeof(server_addr));

    listen(server, 5); 

    int client_size = sizeof(client_addr);
    int client; 
    char buffer[150];

    Player *A = create_player(1, 10); 
    Player *B = create_player(1, 11);

    int playerAThrow = 3, 
        playerBThrow = 0;

    int result;        

    char message_to_client[] = "Hello. Please insert 0 if you want even and 1 otherwhise.\nAlso insert the Id you want and the number you want to play"; 
    
    while(1){
        client = accept(server, (struct sockaddr*)&client_addr, &client_size); 

        recv(client, buffer, sizeof(buffer), 0);
        write(client, message_to_client, strlen(message_to_client));
        sscanf(buffer, "%d %d %d", &B->choice, &B->id, &playerBThrow);
        

        result = decide(A, playerAThrow, B, playerBThrow);

        write(client, result, sizeof(result));


        

        memset(&buffer, 0, sizeof(buffer));
        //close(client);
    }


    return 0; 
}


