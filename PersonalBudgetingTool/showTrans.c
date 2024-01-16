#include "functions.h"

void showTrans(int client, sqlite3 *db1, char *iban)
{
    int ok = 0;
    char *sql = sqlite3_mprintf("SELECT * FROM transactions WHERE iban=?;");
    sqlite3_stmt *stmt;
    int rcsql = sqlite3_prepare_v2(db1, sql, -1, &stmt, NULL);

    if (rcsql != SQLITE_OK)
    {
        fprintf(stderr, "Eroare la pregătirea instrucțiunii SQL: %s\n", sqlite3_errmsg(db1));
        sqlite3_free(sql);
        return;
    }

    sqlite3_bind_text(stmt, 1, iban, -1, SQLITE_STATIC);

    int step;
    char msglong[10000]={0};
    while ((step = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        ok = 1;
        const char *transaction_type = sqlite3_column_text(stmt, 1);
        const char *cost = sqlite3_column_text(stmt, 2);
        char *msgl = sqlite3_mprintf("Transaction type: %s Cost: %s\n", transaction_type, cost);
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