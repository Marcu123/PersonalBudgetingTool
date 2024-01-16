#include "functions.h"

void showCards(int client, sqlite3 *db1, char *username)
{
    int ok = 0;
    char *sql = sqlite3_mprintf("SELECT * FROM accounts where username=?;");
    sqlite3_stmt *stmt;
    int rcsql = sqlite3_prepare_v2(db1, sql, -1, &stmt, NULL);

    if (rcsql != SQLITE_OK)
    {
        fprintf(stderr, "Eroare la pregătirea instrucțiunii SQL: %s\n", sqlite3_errmsg(db1));
        sqlite3_free(sql);
        return;
    }

    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);

    int step;
    char msglong[10000]={0};
    while ((step = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        ok = 1;
        const char *iban = sqlite3_column_text(stmt, 1);
        const char *total_balance = sqlite3_column_text(stmt, 2);
        char *msgl = sqlite3_mprintf("Iban: %s\nAccount balance: %s\n", iban, total_balance);
        strncat(msglong, msgl, sizeof(msglong) - strlen(msglong) - 1); 
        sqlite3_free(msgl);
    }
    if (ok == 1)
    {
        writeToClient(client, msglong);
        memset(msglong, 0, sizeof(msglong));
    }
    else 
    {
        writeToClient(client, "Nu exista niciun card!\n");
        memset(msglong, 0, sizeof(msglong));
    }

    sqlite3_finalize(stmt);
    sqlite3_free(sql);
}
