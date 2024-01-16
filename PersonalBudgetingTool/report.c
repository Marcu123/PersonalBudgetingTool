#include "functions.h"

void report(int client, sqlite3 *db1, char *username) {
    char *sql = sqlite3_mprintf(
    "SELECT CASE "
    "WHEN transaction_type LIKE '%%mancare-ieftina' THEN 'mancare-ieftina' "
    "WHEN transaction_type LIKE '%%mancare-scumpa' THEN 'mancare-scumpa' "
    "WHEN transaction_type LIKE '%%apa)' THEN 'apa' "
    "WHEN transaction_type LIKE '%%garderoba)' THEN 'garderoba' "
    "WHEN transaction_type LIKE '%%jocuri_video)' THEN 'jocuri_video' "
    "WHEN transaction_type LIKE '%%gambling)' THEN 'gambling' "
    "WHEN transaction_type LIKE '%%activitati)' THEN 'activitati' "
    "WHEN transaction_type LIKE '%%depunere' THEN 'activitati' "
    "ELSE 'Alte categorii' "
    "END as category, SUM(cost) "
    "FROM transactions WHERE username=? GROUP BY category;"
);


    sqlite3_stmt *stmt;
    int rcsql = sqlite3_prepare_v2(db1, sql, -1, &stmt, NULL);

    if (rcsql != SQLITE_OK) {
        fprintf(stderr, "Eroare la pregătirea instrucțiunii SQL: %s\n", sqlite3_errmsg(db1));
        sqlite3_free(sql);
        return; 
    }

    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);

    double totalCost = 0;
    char msglong[10000] = ""; 

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        totalCost += sqlite3_column_double(stmt, 1);
    }

    sqlite3_reset(stmt);

    strcat(msglong, "Raportul tranzactiilor:\n");
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        char *category = (char *)sqlite3_column_text(stmt, 0);
        double cost = sqlite3_column_double(stmt, 1);
        double percent = (cost / totalCost) * 100;

        char line[200]={0};
        snprintf(line, sizeof(line), "%s: %.2f%% din total (%.2f)\n", category, percent, cost);
        strcat(msglong, line);
    }

    writeToClient(client, msglong);
    sqlite3_finalize(stmt);
    sqlite3_free(sql);
}
