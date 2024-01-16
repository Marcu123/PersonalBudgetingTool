#include "functions.h"

#define LOGIN "Te-ai logat cu succes! Petru a vedea comenzile si parametrii te rog sa folosesti comanda help\n"
#define BADLOGIN "Username sau parola gresite! Incearca din nou!\n"
#define OTHERUSER "Acest cont este deja folosit! Incearca din nou!\n"

void loginFunction(int client, sqlite3 *db1, char *username, char *password, int *login)
{
    char *sql = sqlite3_mprintf("SELECT logged FROM users WHERE username=? AND password=?;");
    sqlite3_stmt *stmt;
    int rcsql = sqlite3_prepare_v2(db1, sql, -1, &stmt, NULL);

    if (rcsql != SQLITE_OK)
    {
        fprintf(stderr, "Eroare la pregătirea instrucțiunii SQL: %s\n", sqlite3_errmsg(db1));
        sqlite3_free(sql);
        return;
    }

    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password, -1, SQLITE_STATIC);

    int step = sqlite3_step(stmt);

    if (step == SQLITE_ROW)
    {
        int logged = sqlite3_column_int(stmt, 0);
        if (logged == 1)
        {
            writeToClient(client, OTHERUSER);
        }
        else
        {
            writeToClient(client, LOGIN);
            *login = 1;

            char *sql_update = sqlite3_mprintf("UPDATE users SET logged=1 WHERE username=?;");
            sqlite3_stmt *stmt_update;
            if (sqlite3_prepare_v2(db1, sql_update, -1, &stmt_update, NULL) == SQLITE_OK)
            {
                sqlite3_bind_text(stmt_update, 1, username, -1, SQLITE_STATIC);
                sqlite3_step(stmt_update);
                sqlite3_finalize(stmt_update);
            }
            sqlite3_free(sql_update);
        }
    }
    else if (step == SQLITE_DONE)
    {
        writeToClient(client, BADLOGIN);
    }

    sqlite3_finalize(stmt);
    sqlite3_free(sql);
}
