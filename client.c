#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include<ctype.h>


#define min(a,b) a<b?a:b
#define BUFFER_SIZE 1024

int write_file(int clientsocket, FILE *fp,int size){
    
    char buffer[BUFFER_SIZE] = {0};
    memset(buffer,'0',BUFFER_SIZE);
    int i=0;
    int n = recv(clientsocket,buffer,sizeof(buffer),0);
    while(n>0 && buffer[i] != '\0' && i<size){
        putc(buffer[i],fp);
        // printf("%d ",i);
        i++;
    }
    return n;
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

    char buffer[BUFFER_SIZE];

    memset(buffer,'\0',BUFFER_SIZE);

    int n = read(Clientsocket,buffer,BUFFER_SIZE);
    printf("msg received :: %s::",buffer);

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
    printf("filename sent \n");

    n = recv(Clientsocket,buffer,BUFFER_SIZE,0);
    if(n<0){
        perror("recv failed");
        exit(1);
    }
    printf("size of file ::%d::\n",atoi(buffer));

    
    
    fp = fopen("newfile.c","w");
    if(fp==NULL){
        perror("File Not Found");
        exit(1);
    }

    int file_data_remaining = atoi(buffer);
    while(file_data_remaining>0){
        // memset(buffer,'0',sizeof(buffer));
        // n = recv(Clientsocket,buffer,sizeof(buffer),0);
        // fwrite(buffer,sizeof(char),n,fp);
        n = write_file(Clientsocket,fp,file_data_remaining);
        if(n<0 ){
            perror("Recv failed");
            exit(1);
        }
        if(n == 0)
        break;
        // fprintf(fp,"%s",buffer);
        n = min(file_data_remaining,n);
        file_data_remaining -= n;
        printf("[+] %d bytes received and %d remaining\n",n,file_data_remaining);
    }

    fclose(fp);
    close(Clientsocket);
    printf("Socket Closed\n");
    return 0;
}