#include "functions.h"
void showTotalBalance(int client, sqlite3 *db1, char *username, char *password)
{
    char *sql = sqlite3_mprintf("SELECT total_balance FROM users WHERE username=? and password=?;");
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

    int step;
    char msglong[10000] = {0};
    while ((step = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        const char *total_balance = sqlite3_column_text(stmt, 0);
        char *msgl = sqlite3_mprintf("Total balance: %s\n", total_balance);
        strcat(msglong, msgl);
        writeToClient(client, msgl);
        sqlite3_free(msgl);
    }
    sqlite3_finalize(stmt);
    sqlite3_free(sql);
}