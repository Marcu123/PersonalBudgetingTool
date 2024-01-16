#include "functions.h"

void writeToClient(int client, const char *msg_sv)
{
    size_t msg_size = strlen(msg_sv);
    size_t msg_sizen = htonl(msg_size);

    printf("[server]Trimitem mesajul inapoi...%s\n", msg_sv);

    if (write(client, &msg_sizen, sizeof(size_t)) <= 0)
    {
        perror("[server]Error writing to client.\n");
        return;
    }

    if (write(client, msg_sv, msg_size) <= 0)
    {
        perror("[server]Error writing to client.\n");
    }
}