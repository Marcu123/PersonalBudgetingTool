#include "functions.h"

#define INSERTTRANSSUCCESS "Tranzactie inserata cu succes!\n"
#define BADCOMMAND "Comanda nu este valida! Incearca din nou!\n"

void insertTrans(int client, sqlite3 *db1, sqlite3 *db2, char *iban, char *transaction_type, char *cost, char *username)
{
     int costint = atoi(cost);
    char updated_transaction_type[100]={0};

    if (strstr(transaction_type, "mancare") != NULL) {
        snprintf(updated_transaction_type, sizeof(updated_transaction_type), "%s %s", 
                 transaction_type, costint >= 50 ? "mancare-scumpa" : "mancare-ieftina");
    } else {
        strcpy(updated_transaction_type, transaction_type);
    }

    char *sql = "INSERT INTO transactions VALUES(?, ?, ?, ?);";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db1, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Eroare la pregătirea instrucțiunii SQL: %s\n", sqlite3_errmsg(db1));
        writeToClient(client, BADCOMMAND);
        return;
    }

    sqlite3_bind_text(stmt, 1, iban, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, updated_transaction_type, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, cost, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, username, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        writeToClient(client, BADCOMMAND);
        sqlite3_finalize(stmt);
        return;
    }
    sqlite3_finalize(stmt);

    char *sqlac = "UPDATE accounts SET account_balance = account_balance - ? WHERE iban = ?;";
    sqlite3_stmt *stmtac;
    if (sqlite3_prepare_v2(db1, sqlac, -1, &stmtac, NULL) != SQLITE_OK) {
        fprintf(stderr, "Eroare la pregătirea instrucțiunii SQL: %s\n", sqlite3_errmsg(db1));
        return;
    }

    sqlite3_bind_text(stmtac, 1, cost, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmtac, 2, iban, -1, SQLITE_STATIC);

    if (sqlite3_step(stmtac) != SQLITE_DONE) {
        writeToClient(client, BADCOMMAND);
        sqlite3_finalize(stmtac);
        return;
    }
    sqlite3_finalize(stmtac);

    char *sqlu = "UPDATE users SET total_balance = total_balance - ? WHERE username = ?;";
    sqlite3_stmt *stmtu;
    if (sqlite3_prepare_v2(db1, sqlu, -1, &stmtu, NULL) != SQLITE_OK) {
        fprintf(stderr, "Eroare la pregătirea instrucțiunii SQL: %s\n", sqlite3_errmsg(db1));
        return;
    }

    sqlite3_bind_text(stmtu, 1, cost, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmtu, 2, username, -1, SQLITE_STATIC);

    if (sqlite3_step(stmtu) != SQLITE_DONE) {
        writeToClient(client, BADCOMMAND);
        sqlite3_finalize(stmtu);
        return;
    }
    sqlite3_finalize(stmtu);


    writeToClient(client, INSERTTRANSSUCCESS);
}