#include "functions.h"

#define CARDDELETED "Card sters cu succes!\n"
#define BADCOMMAND "Comanda nu este valida! Incearca din nou!\n"

void deleteCard(int client, sqlite3 *db1, char *iban, char *username) {

    char *sql = "SELECT account_balance FROM accounts WHERE iban = ?;";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db1, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Error preparing SELECT statement: %s\n", sqlite3_errmsg(db1));
        return;
    }

    sqlite3_bind_text(stmt, 1, iban, -1, SQLITE_STATIC);
    double account_balance = 0.0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        account_balance = sqlite3_column_double(stmt, 0);
    } else {
        writeToClient(client, BADCOMMAND);
        sqlite3_finalize(stmt);
        return;
    }
    sqlite3_finalize(stmt);

    sql = "UPDATE users SET total_balance = total_balance - ? WHERE username = ?;";
    if (sqlite3_prepare_v2(db1, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Error preparing UPDATE statement: %s\n", sqlite3_errmsg(db1));
        return;
    }

    sqlite3_bind_double(stmt, 1, account_balance);
    sqlite3_bind_text(stmt, 2, username, -1, SQLITE_STATIC);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        writeToClient(client, BADCOMMAND);
        sqlite3_finalize(stmt);
        return;
    }
    sqlite3_finalize(stmt);

    sql = "DELETE FROM transactions WHERE iban = ?;";
    if (sqlite3_prepare_v2(db1, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Error preparing DELETE statement: %s\n", sqlite3_errmsg(db1));
        return;
    }

    sqlite3_bind_text(stmt, 1, iban, -1, SQLITE_STATIC);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        writeToClient(client, BADCOMMAND);
        sqlite3_finalize(stmt);
        return;
    }
    sqlite3_finalize(stmt);

    sql = "DELETE FROM accounts WHERE iban = ?;";
    if (sqlite3_prepare_v2(db1, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Error preparing DELETE statement: %s\n", sqlite3_errmsg(db1));
        return;
    }

    sqlite3_bind_text(stmt, 1, iban, -1, SQLITE_STATIC);
    if (sqlite3_step(stmt) == SQLITE_DONE) {
        writeToClient(client, CARDDELETED);
    } else {
        writeToClient(client, BADCOMMAND);
    }
    sqlite3_finalize(stmt);

}
