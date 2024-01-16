#include "functions.h"
void showAllTrans(int client, sqlite3 *db1, char *username)
{
    int ok = 0;
    char *sql = sqlite3_mprintf("SELECT * FROM transactions WHERE username=?;");
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
        const char *iban_tr = sqlite3_column_text(stmt, 0);
        const char *tr_type = sqlite3_column_text(stmt, 1);
        const char *cost_per_tr = sqlite3_column_text(stmt, 2);
        char *msgl = sqlite3_mprintf("IBAN: %s, Transaction type: %s, Cost: %s\n", iban_tr, tr_type, cost_per_tr);
        strcat(msglong, msgl);
        sqlite3_free(msgl);
    }
    if (ok == 1)
    {
        writeToClient(client, msglong);
        memset(msglong, 0, sizeof(msglong));
    }
    else
    {
        writeToClient(client, "Nu exista nicio tranzactie!\n");
        memset(msglong, 0, sizeof(msglong));
    }
    sqlite3_finalize(stmt);
    sqlite3_free(sql);
}