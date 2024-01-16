#include "functions.h"

#define INSERTCARDSUCCESS "Card inserat cu succes!\n"
#define BADCOMMAND "Comanda nu este valida! Incearca din nou!\n"

void insertCard(int client, sqlite3 *db1, char *username, char *iban, char *total_balance)
{
    char *sql = sqlite3_mprintf("INSERT INTO accounts VALUES(?,?,?);");
    sqlite3_stmt *stmt;
    int rcsql = sqlite3_prepare_v2(db1, sql, -1, &stmt, NULL);

    if (rcsql != SQLITE_OK)
    {
        fprintf(stderr, "Eroare la pregătirea instrucțiunii SQL: %s\n", sqlite3_errmsg(db1));
        sqlite3_free(sql);
        return;
    }

    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, iban, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, total_balance, -1, SQLITE_STATIC);

    char *sqlu=sqlite3_mprintf("UPDATE users SET total_balance=total_balance+? WHERE username=?;");
    sqlite3_stmt *stmtu;
    int rcsqlu = sqlite3_prepare_v2(db1, sqlu, -1, &stmtu, NULL);

    if (rcsqlu != SQLITE_OK)
    {
        fprintf(stderr, "Eroare la pregătirea instrucțiunii SQL: %s\n", sqlite3_errmsg(db1));
        sqlite3_free(sqlu);
        return;
    }

    sqlite3_bind_text(stmtu, 1, total_balance, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmtu, 2, username, -1, SQLITE_STATIC);


    int step = sqlite3_step(stmt);
    if (step == SQLITE_DONE)
    {
        writeToClient(client, INSERTCARDSUCCESS);
    }
    else
    {
        writeToClient(client, BADCOMMAND);
    }
    sqlite3_finalize(stmt);
    sqlite3_free(sql);

    int stepu = sqlite3_step(stmtu);
    
    sqlite3_finalize(stmtu);
    sqlite3_free(sqlu);
    
}