#include "functions.h"

#define INSERTTRANSSUCCESS "Tranzactie inserata cu succes!\n"
#define BADCOMMAND "Comanda nu este valida! Incearca din nou!\n"
void addMoney(int client, sqlite3 *db1, char *iban, char *cost, char *username)
{
    char *sql = sqlite3_mprintf("INSERT INTO transactions VALUES(?,?,?,?);");
    sqlite3_stmt *stmt;
    int rcsql = sqlite3_prepare_v2(db1, sql, -1, &stmt, NULL);

    if (rcsql != SQLITE_OK)
    {
        fprintf(stderr, "Eroare la pregătirea instrucțiunii SQL: %s\n", sqlite3_errmsg(db1));
        sqlite3_free(sql);
        return;
    }

    sqlite3_bind_text(stmt, 1, iban, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, "depunere", -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, cost, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, username, -1, SQLITE_STATIC);

    char *sqlac=sqlite3_mprintf("UPDATE accounts SET account_balance=account_balance+? WHERE iban=?;");
    sqlite3_stmt *stmtac;
    int rcsqlac = sqlite3_prepare_v2(db1, sqlac, -1, &stmtac, NULL);

    if (rcsqlac != SQLITE_OK)
    {
        fprintf(stderr, "Eroare la pregătirea instrucțiunii SQL: %s\n", sqlite3_errmsg(db1));
        sqlite3_free(sqlac);
        return;
    }

    sqlite3_bind_text(stmtac, 1, cost, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmtac, 2, iban, -1, SQLITE_STATIC);

    char *sqlu=sqlite3_mprintf("UPDATE users SET total_balance=total_balance+? WHERE username=?;");
    sqlite3_stmt *stmtu;
    int rcsqlu = sqlite3_prepare_v2(db1, sqlu, -1, &stmtu, NULL);

    if (rcsqlu != SQLITE_OK)
    {
        fprintf(stderr, "Eroare la pregătirea instrucțiunii SQL: %s\n", sqlite3_errmsg(db1));
        sqlite3_free(sqlu);
        return;
    }

    sqlite3_bind_text(stmtu, 1, cost, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmtu, 2, username, -1, SQLITE_STATIC);

    int step = sqlite3_step(stmt);
    if (step == SQLITE_DONE)
    {
        writeToClient(client, INSERTTRANSSUCCESS);
    }
    else
    {
        writeToClient(client, BADCOMMAND);
    }
    sqlite3_finalize(stmt);
    sqlite3_free(sql);

    int stepac = sqlite3_step(stmtac);
    
    sqlite3_finalize(stmtac);
    sqlite3_free(sqlac);

    int stepu = sqlite3_step(stmtu);
    
    sqlite3_finalize(stmtu);
    sqlite3_free(sqlu);
    
}