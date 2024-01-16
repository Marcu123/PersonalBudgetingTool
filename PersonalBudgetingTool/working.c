#include "functions.h"

void working(void *arg)
{
    int login = 0;
    struct thData tdL;
    tdL = *((struct thData *)arg);

    sqlite3 *db1;
    int rc1 = sqlite3_open("bankAccount.db", &db1);
    if (rc1 != SQLITE_OK)
    {
        perror("Nu s-a putut deschide baza de date bankAccount.db.\n");
        return;
    }

    sqlite3 *db2;
    int rc2 = sqlite3_open("trType.db", &db2);
    if (rc2 != SQLITE_OK)
    {
        perror("Nu s-a putut deschide baza de date trType.db.\n");
        return;
    }

    char *msg;
    size_t msg_size;
    size_t msg_sizen;

    writeToClient(tdL.cl, WELCOME);

    char *command;
    char *username;
    char *password;
    char *iban;
    char *transaction_type;
    char *cost;
    char *total_balance;

    while (1)
    {
        if (login == 0)
        {
            if (read(tdL.cl, &msg_sizen, sizeof(size_t)) < 0)
            {
                perror("[server]Eroare la read() de la client.\n");
            }

            msg_size = ntohl(msg_sizen);
            msg = (char *)malloc(msg_size);

            if (read(tdL.cl, msg, msg_size) < 0)
            {
                perror("[server]Eroare la read() de la client.\n");
            }

            char *p = strtok(msg, ":");
            if (p != NULL)
            {
                command = (char *)malloc(strlen(p) + 1);
                strcpy(command, p);
            }

            p = strtok(NULL, " ");
            if (p != NULL)
            {
                username = (char *)malloc(strlen(p) + 1);
                strcpy(username, p);
            }

            p = strtok(NULL, " ");
            if (p != NULL)
            {
                password = (char *)malloc(strlen(p) + 1);
                strcpy(password, p);
            }

            if (strcmp(command, "login") == 0)
            {
                loginFunction(tdL.cl, db1, username, password, &login);
                free(msg);
                free(command);
            }
            else if (strcmp(command, "register") == 0)
            {

                if (username != NULL && password != NULL)
                {
                    registerIbanNull(tdL.cl, db1, username, password);

                    free(msg);
                    free(command);
                    free(username);
                    free(password);
                }
                else
                {
                    writeToClient(tdL.cl, BADCOMMAND);
                    free(msg);
                }
            }
            else if (strcmp(msg, "exit") == 0)
            {
                writeToClient(tdL.cl, EXIT);
                free(msg);
                return;
            }
            else
            {
                writeToClient(tdL.cl, BADCOMMAND);
                free(msg);
            }
        }
        else if (login == 1)
        {
            if (read(tdL.cl, &msg_sizen, sizeof(size_t)) < 0)
            {
                perror("[server]Eroare la read() de la client.\n");
            }

            msg_size = ntohl(msg_sizen);
            msg = (char *)malloc(msg_size);

            if (read(tdL.cl, msg, msg_size) < 0)
            {
                perror("[server]Eroare la read() de la client.\n");
            }

            if (strcmp(msg, "exit") == 0)
            {
                exitFunction(tdL.cl, db1, username);
                login=0;
                free(msg);
                break;
            }
            else if (strcmp(msg, "help") == 0)
            {
                writeToClient(tdL.cl, HELPMSG);
                free(msg);
            }
            else
            {
                char *r = strtok(msg, ":");
                if (r != NULL)
                {
                    command = (char *)malloc(strlen(r) + 1);
                    strcpy(command, r);
                }

                if (strcmp(command, "logout") == 0)
                {
                    
                    logout(tdL.cl, db1, username);
                    login = 0;
                    free(msg);
                }
                else if (strcmp(command, "insertcard") == 0)
                {
                    r = strtok(NULL, " ");
                    if (r != NULL)
                    {
                        iban = (char *)malloc(strlen(r) + 1);
                        strcpy(iban, r);
                    }

                    r = strtok(NULL, " ");
                    if (r != NULL)
                    {
                        total_balance = (char *)malloc(strlen(r) + 1);
                        strcpy(total_balance, r);
                    }

                    if (iban != NULL && total_balance != NULL)
                    {
                        insertCard(tdL.cl, db1, username, iban, total_balance);
                    }
                    free(msg);
                    free(command);
                    free(iban);
                    free(total_balance);
                }
                else if (strcmp(msg, "inserttrans") == 0)
                {
                    r = strtok(NULL, " ");
                    if (r != NULL)
                    {
                        iban = (char *)malloc(strlen(r) + 1);
                        strcpy(iban, r);
                    }

                    r = strtok(NULL, " ");
                    if (r != NULL)
                    {
                        transaction_type = (char *)malloc(strlen(r) + 1);
                        strcpy(transaction_type, r);
                    }

                    r = strtok(NULL, " ");
                    if (r != NULL)
                    {
                        cost = (char *)malloc(strlen(r) + 1);
                        strcpy(cost, r);
                    }

                    if (iban != NULL && transaction_type != NULL && cost != NULL)
                    {
                        insertTrans(tdL.cl, db1, db2, iban, transaction_type, cost, username);
                    }

                    free(msg);
                    free(command);
                    free(iban);
                    free(transaction_type);
                    free(cost);
                }
                else if (strcmp(command, "showcards") == 0)
                {
                    showCards(tdL.cl, db1, username);
                    free(msg);
                    free(command);
                }
                else if (strcmp(command, "showtrans") == 0)
                {
                    r = strtok(NULL, " ");
                    if (r != NULL)
                    {
                        iban = (char *)malloc(strlen(r) + 1);
                        strcpy(iban, r);
                    }

                    if (iban != NULL)
                    {
                        showTrans(tdL.cl, db1, iban);
                        free(msg);
                        free(command);
                        free(iban);
                    }
                }
                else if (strcmp(command, "showbalance") == 0)
                {
                    r = strtok(NULL, " ");
                    if (r != NULL)
                    {
                        iban = (char *)malloc(strlen(r) + 1);
                        strcpy(iban, r);
                    }

                    if (iban != NULL)
                    {
                        showBalance(tdL.cl, db1, iban);
                        free(msg);
                        free(command);
                        free(iban);
                    }
                }
                else if (strcmp(command, "addmoney") == 0)
                {
                    r = strtok(NULL, " ");
                    if (r != NULL)
                    {
                        iban = (char *)malloc(strlen(r) + 1);
                        strcpy(iban, r);
                    }

                    r = strtok(NULL, " ");
                    if (r != NULL)
                    {
                        cost = (char *)malloc(strlen(r) + 1);
                        strcpy(cost, r);
                    }

                    if (iban != NULL)
                    {
                        addMoney(tdL.cl, db1, iban, cost, username);
                        free(msg);
                        free(command);
                        free(iban);
                        free(cost);
                    }
                }
                else if (strcmp(command, "deletecard") == 0)
                {
                    r = strtok(NULL, " ");
                    if (r != NULL)
                    {
                        iban = (char *)malloc(strlen(r) + 1);
                        strcpy(iban, r);
                    }

                    if (iban != NULL)
                    {
                        deleteCard(tdL.cl, db1, iban, username);
                    }
                    free(msg);
                    free(command);
                    free(iban);
                }
                else if (strcmp(command, "showalltrans") == 0)
                {
                    showAllTrans(tdL.cl, db1, username);
                    free(msg);
                    free(command);
                }
                else if (strcmp(command, "showtotalbalance") == 0)
                {

                    showTotalBalance(tdL.cl, db1, username, password);
                    free(msg);
                    free(command);
                }
                else if (strcmp(command, "save") == 0)
                {
                    save(tdL.cl, db1, username);
                    free(msg);
                    free(command);
                }
                else if (strcmp(command,"report")==0)
                {
                    report(tdL.cl,db1,username);
                    free(msg);
                    free(command);
                }
                else
                {
                    writeToClient(tdL.cl, BADCOMMAND);
                    free(msg);
                    free(command);
                }
            }
        }
        else
        {
            writeToClient(tdL.cl, BADCOMMAND);
            free(msg);
        }
    }
}