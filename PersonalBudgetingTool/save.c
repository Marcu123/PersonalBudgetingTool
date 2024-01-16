#include "functions.h"

void save(int client, sqlite3 *db1, char *username)
{
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
    "END as category, SUM(cost) as total_cost "
    "FROM transactions WHERE username=? GROUP BY category ORDER BY total_cost DESC LIMIT 1;");

sqlite3_stmt *stmt;
int rcsql = sqlite3_prepare_v2(db1, sql, -1, &stmt, NULL);

if (rcsql != SQLITE_OK) {
    fprintf(stderr, "Eroare la pregătirea instrucțiunii SQL: %s\n", sqlite3_errmsg(db1));
    sqlite3_free(sql);
    return;
}

sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);

char maxCategory[200] = "";
double maxCost = 0;

if (sqlite3_step(stmt) == SQLITE_ROW) {
    strncpy(maxCategory, (char *)sqlite3_column_text(stmt, 0), sizeof(maxCategory) - 1);
    maxCost = sqlite3_column_double(stmt, 1);
}

char msglong[10000]={0};
snprintf(msglong, sizeof(msglong), "Cea mai mare cheltuiala a fost in categoria '%s' cu suma de %.2f\n", maxCategory, maxCost);

char recommendation[10000]={0};
strcat(recommendation, msglong);
if(strcmp(maxCategory,"mancare-scumpa")==0)
{
    snprintf(recommendation, sizeof(recommendation), "Recomandare: Incearca sa mananci mai sanatos si sa nu mai cheltuiesti atat de mult pe mancare!\n");
    strcat(msglong, recommendation);
}
else if(strcmp(maxCategory,"mancare-ieftina")==0)
{
    snprintf(recommendation, sizeof(recommendation), "Recomandare: Incearca sa mananci mai sanatos!\n");
    strcat(msglong, recommendation);
}
else if(strcmp(maxCategory,"apa")==0)
{
    snprintf(recommendation, sizeof(recommendation), "Recomandare: Incearca sa cumperi apa mai ieftina sau sa incerci sa gasesti promotii!\n");
    strcat(msglong, recommendation);
}
else if(strcmp(maxCategory,"garderoba")==0)
{
    snprintf(recommendation, sizeof(recommendation), "Recomandare: Incearca sa nu mai cheltuiesti atat de mult pe haine scumpe. Ai putea incerca ceva mai ieftin dar tot cu gust!\n");
    strcat(msglong, recommendation);
}
else if(strcmp(maxCategory,"jocuri_video")==0)
{
    snprintf(recommendation, sizeof(recommendation), "Recomandare: Incearca sa nu mai cheltuiesti atat de mult pe jocuri video. Pot deveni o dependenta si pierzi mult timp cu ele!\n");
    strcat(msglong, recommendation);
}
else if(strcmp(maxCategory,"gambling")==0)
{
    snprintf(recommendation, sizeof(recommendation), "Recomandare: Incearca sa nu mai cheltuiesti atat de mult pe gambling! Jocurile de noroc sunt o problema pentru societate si poti ajunge in multe situatii rele.\n");
    strcat(msglong, recommendation);
}
else if(strcmp(maxCategory,"activitati")==0)
{
    snprintf(recommendation, sizeof(recommendation), "Recomandare: Incearca sa nu mai cheltuiesti atat de mult pe activitati! Incearca sa te limitezi\n");
    strcat(msglong, recommendation);
}
else if(strcmp(maxCategory,"tigari")==0)
{
    snprintf(recommendation, sizeof(recommendation), "Recomandare: Incearca sa nu mai cheltuiesti atat de mult pe tigari! Incearca sa te lasi de fumat! Fumatul ucide!\n");
    strcat(msglong, recommendation);
}
else if(strcmp(maxCategory,"alcool")==0)
{
    snprintf(recommendation, sizeof(recommendation), "Recomandare: Incearca sa nu mai cheltuiesti atat de mult pe alcool! Incearca sa te limitezi! Poti avea probleme grave de sanatate daca continui cu alcoolul!\n");
    strcat(msglong, recommendation);
}
else if(strcmp(maxCategory,"depunere")==0)
{
    snprintf(recommendation, sizeof(recommendation), "Recomandare: Este bine cu banii! Sper sa nu fie doar o schema de a ma pacali si sa cred ca nu cheltuiesti atat de mult!:)\n");
    strcat(msglong, recommendation);
}
else
{
    snprintf(recommendation, sizeof(recommendation), "Recomandare: Incearca sa nu mai cheltuiesti atat de mult pe alte categorii!\n");
    strcat(msglong, recommendation);
}

writeToClient(client, msglong);

sqlite3_finalize(stmt);
sqlite3_free(sql);
}
