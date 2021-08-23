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
    char filename[20];

    printf("Enter the File name to transfer :");
    scanf("%s",filename);

    n = send(Clientsocket,filename,strlen(filename),0);
    if(n<0){
        perror("Send failed");
        exit(1);
    }
    printf("file sent \n");

    n = read(Clientsocket,buffer,255);
    printf("size of file ::%d::",atoi(buffer));

    fp = fopen("newfile.txt","w");
    if(fp==NULL){
        perror("File Not Found");
        exit(1);
    }
    int file_data_remaining = atoi(buffer);

    while(file_data_remaining>0){
        n = recv(Clientsocket,buffer,255,0);
        if(n<0){
            perror("Recv failed");
            exit(1);
        }
        fwrite(buffer,sizeof(char),n,fp);
        file_data_remaining -= n;
        printf("[+] %d bytes received and %d remaining\n",n,file_data_remaining);
    }

    fclose(fp);
    close(Clientsocket);
    printf("Socket Closed\n");
    return 0;
}