#include "functions.h"

#define REGISTER "Cont creat cu succes! Te poti loga acum cu noul cont.\n"
#define BADREGISTER "Username deja existent! Incearca din nou folosind alt username!\n"

void registerIbanNull(int client, sqlite3 *db1, char *username, char *password)
{
    char *sql = sqlite3_mprintf("INSERT INTO users VALUES(?,?,?,?);");
    sqlite3_stmt *stmt;
    int rcsql = sqlite3_prepare_v2(db1, sql, -1, &stmt, NULL);

    if (rcsql != SQLITE_OK)
    {
        fprintf(stderr, "Eroare la pregătirea instrucțiunii SQL: %s\n", sqlite3_errmsg(db1));
        sqlite3_free(sql);
        return;
    }

    char* sold="0";
    char* logged="0";
    
    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, sold, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, logged, -1, SQLITE_STATIC);

    int step = sqlite3_step(stmt);
    if (step == SQLITE_DONE)
    {
        writeToClient(client, REGISTER);
    }
    else
    {
        writeToClient(client, BADREGISTER);
    }
    sqlite3_finalize(stmt);
    sqlite3_free(sql);
}