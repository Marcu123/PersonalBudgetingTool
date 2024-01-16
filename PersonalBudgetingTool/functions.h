#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <stdint.h>
#include <sqlite3.h>

typedef struct thData{
    int idThread;
    int cl;
} thData;

#define PORT 2909
#define HELPMSG "Pentru a insera un card nou foloseste comanda insertcard:iban-card balance\nPentru a insera o tranzactie foloseste comanda inserttrans:iban-card transaction-type cost. Transaction-type poate fi orice nume de brand, dar urmat de o categorie general. e.x. McDonalds(mancare/apa/garderoba/gambling/jocuri_video/activitati)\nPentru a vedea toate cardurile foloseste comanda showcards\nPentru a vedea toate tranzactiile din cont foloseste comanda showalltrans\nPentru a vedea toate tranzactiile unui card foloseste comanda showtrans:iban-card\nPentru a vedea balanta unui card foloseste comanda showbalance: iban-card\nPentru a veada balanta totala din cont foloseste showtotalbalance\nPentru a adauga bani in cont foloseste comanda addmoney:iban-card suma\nPentru a sterge un card foloseste comanda deletecard:iban-card\nPentru a te deloga foloseste comanda logout\nPentru a vedea o recomandare in a salva mai multi bani foloseste comanda save\nPentru a vedea un raport cu cel mai dominant obicei de cheltuieli foloseste comanda report\nPentru a iesi direct din aplicatie foloseste comanda exit\n"
#define WELCOME "Bine ai venit!\nTe rog sa te autentifici folosind comanda de tipul login:username password.\nDaca nu ai cont, te rog sa te inregistrezi folosind comanda de tipul register:username password.\n"
#define LOGOUT "Te-ai delogat cu succes!\n"
#define BADCOMMAND "Comanda nu este valida! Incearca din nou!\n"
#define EXIT "exit\n"
#define DISCONNECTED "Te-ai deconectat!\n"
#define LOGGED "Esti logat!\n"

extern int errno;

void showTotalBalance(int client, sqlite3 *db1, char *username, char *password);

void showAllTrans(int client, sqlite3 *db1, char *username);

void deleteCard(int client, sqlite3 *db1, char *iban, char *username);

void addMoney(int client, sqlite3 *db1, char *iban, char *cost, char *username);

void showBalance(int client, sqlite3 *db1, char *iban);

void showTrans(int client, sqlite3 *db1, char *iban);

void showCards(int client, sqlite3 *db1, char *username);

void report(int client, sqlite3 *db1, char *username);

void save(int client, sqlite3 *db1, char *username);

void insertTrans(int client, sqlite3 *db1, sqlite3 *db2, char *iban, char *transaction_type, char *cost, char *username);

void insertCard(int client, sqlite3 *db1, char *username, char *iban, char *total_balance);


void registerIbanNull(int client, sqlite3 *db1, char *username, char *password);

void loginFunction(int client, sqlite3 *db1, char *username, char *password, int *login);

void writeToClient(int client, const char *msg_sv);

static void *treat(void *arg);

void working(void *arg);

void logout(int client, sqlite3 *db1, char *username);

void exitFunction(int client, sqlite3 *db1, char *username);

#endif