#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>

#define BUFFER_SIZE 1024

void write_file(int sockfd){
    int n;
    FILE *fp;

    char *filename = "filerev.txt";
    char buffer[BUFFER_SIZE];


    fp = fopen(filename, "w");
    while(1){
        n = recv(sockfd,buffer,BUFFER_SIZE,0);
        if(n <= 0)
            break;
        fprintf(fp,"%s",buffer);
        bzero(buffer,BUFFER_SIZE);
    }
    return;
}


int main(int argc, char *argv[]){
    
    
    int Serversocket = socket(AF_INET,SOCK_STREAM,0);

    struct sockaddr_in serverAddr,clientAddr;

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(9800);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(Serversocket,(struct sockaddr *)&serverAddr,sizeof(serverAddr))<0){
        perror("Binding Failed");
        return -1;
    }

    if(listen(Serversocket,2)<0){
        perror("Listen failed");
        return -1;
    }

    printf("FTP Server Started on port 9800...\n");

    int clientlen = sizeof(clientAddr);
    int clientsocket = accept(Serversocket,(struct sockaddr *)&clientAddr,&clientlen);
    if(clientsocket <0)
        perror("accept failed");
    
    int n = write(clientsocket,"Connected check\n",strlen("Connected check\n"));

    char buffer[255];

    write_file(clientsocket);
    printf("File received\n");

    close(clientsocket);
    close(Serversocket);

    printf("Server CLosed \n");
    return 0;    
}