#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>

extern int errno;

int port;

int main(int argc, char *argv[])
{
    int sd;
    struct sockaddr_in server;

    char *msg;
    size_t msg_size;
    size_t msg_sizen;

    if (argc != 3)
    {
        printf("Sintaxa: %s <adresa_server> <port>\n", argv[0]);
        return -1;
    }

    port = atoi(argv[2]);

    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Eroare la socket().\n");
        return errno;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(argv[1]);
    server.sin_port = htons(port);

    if (connect(sd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
    {
        perror("[client]Eroare la connect().\n");
        return errno;
    }

    while (1)
    {
        if (read(sd, &msg_sizen, sizeof(size_t)) < 0)
        {
            perror("[client]Eroare la read().\n");
            return errno;
        }
        msg_size = ntohl(msg_sizen);
        msg = (char *)malloc(msg_size+1);
        if (read(sd, msg, msg_size) < 0)
        {
            perror("[client]Eroare la read().\n");
            return errno;
        }
        msg[msg_size] = '\0';
        if (strcmp(msg, "exit\n\0") == 0)
        {
            printf("Te vei deconecta de la terminal in:\n");
            printf("3\n");
            printf("2\n");
            printf("1\n");
            fflush(stdout);
            break;
        }
        else
        {
            printf("%s\n", msg);
            fflush(stdout);
        }

        
        getline(&msg, &msg_size, stdin);
        msg_size = strlen(msg);
        msg[msg_size - 1] = '\0';
        msg_sizen = htonl(msg_size);

        if (write(sd, &msg_sizen, sizeof(size_t)) <= 0)
        {
            perror("[client]Eroare la write().\n");
            return errno;
        }

        if (write(sd, msg, msg_size) <= 0)
        {
            perror("[client]Eroare la write().\n");
            return errno;
        }
        
       free(msg);
    }

    close(sd);
}