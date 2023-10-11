#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>


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

    while(1){
        client = accept(server, (struct sockaddr*)&client_addr, &client_size); 

        recv(client, buffer, sizeof(buffer), 0);

        printf("\nMessage sucessfully received from user %d: %s\n", client_addr.sin_addr.s_addr, buffer); 

        memset(&buffer, 0, sizeof(buffer));
        close(client);
    }


    return 0; 
}