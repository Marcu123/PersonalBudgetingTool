#include "functions.h"

#define LOGOUT "Te-ai delogat cu succes!\n"

void logout(int client, sqlite3 *db1, char *username)
{
    
    char *sql = sqlite3_mprintf("UPDATE users SET logged=0 WHERE username=?;");
    sqlite3_stmt *stmt;
    int rcsql = sqlite3_prepare_v2(db1, sql, -1, &stmt, NULL);
    
    
    if (rcsql != SQLITE_OK)
    {
        fprintf(stderr, "Eroare la pregătirea instrucțiunii SQL: %s\n", sqlite3_errmsg(db1));
        sqlite3_free(sql);
        return;
    }

    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
    
    int step = sqlite3_step(stmt);
    if (step == SQLITE_DONE)
    {
        writeToClient(client, LOGOUT);
    }
    sqlite3_finalize(stmt);
    sqlite3_free(sql);
}