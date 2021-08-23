#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include<ctype.h>

#define BUFFER_SIZE 4096

void send_file(int sockfd, FILE *fp){
    int n;
    char data[BUFFER_SIZE] = {0};

    while(fgets(data, BUFFER_SIZE, fp) != NULL){
        if(send(sockfd, data, strlen(data), 0) < 0){
            perror("send failed");
            exit(1);
        }
        bzero(data, BUFFER_SIZE);
    }
}

int main(int argc, char *argv[])
{
    int Clientsocket = socket(AF_INET,SOCK_STREAM,0);

    struct sockaddr_in clientAddr;

    clientAddr.sin_family = AF_INET;
    clientAddr.sin_port = htons(9800);
    clientAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    if(connect(Clientsocket,(struct sockaddr *)&clientAddr,sizeof(clientAddr))<0){
        perror("Connect failed");
        return -1;
    }

    printf("Connection established...\n");

    char buffer[255];

    int n = read(Clientsocket,buffer,255);
    printf("msg received :: %s",buffer);

    buffer[n]='\0'; //clear buffer

    FILE *fp;