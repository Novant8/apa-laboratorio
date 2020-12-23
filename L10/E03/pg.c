#include <stdio.h>
#include <stdlib.h>

#include "pg.h"

/**
 * La funzione aggiorna le statistiche di un personaggio.
 * Chiamata quando viene modificato l'equipaggiamento del personaggio.
 */ 
static void equipStats_update(pg_t* pgp, invArray_t invArray) {
    stat_t eq_stat = empty_stats();
    int inv_idx;
    inv_t* item;

    for(int i=0; i<equipArray_inUse(pgp->equip); i++) {
        inv_idx = equipArray_getEquipByIndex(pgp->equip, i);
        item = invArray_getByIndex(invArray, inv_idx);
        eq_stat = stat_sum(eq_stat, item->stat);
    }

    pgp->eq_stat = eq_stat;
}

int pg_read(FILE *fp, pg_t *pgp) {
    if(pgp == NULL)
        return 0;
    int res = fscanf(fp, "%s %s %s", pgp->cod, pgp->nome, pgp->classe);
    res = res == 3 && stat_read(fp, &pgp->b_stat);
    return res;
}

void pg_clean(pg_t *pgp) {
    if(pgp == NULL)
        return;
    equipArray_free(pgp->equip);
}

void pg_print(FILE *fp, pg_t *pgp, invArray_t invArray) {
    if(pgp == NULL) {
        printf("Personaggio invalido.\n");
        return;
    }
    fprintf(fp, "%s %s %s ", pgp->cod, pgp->nome, pgp->classe);
    stat_print(fp, &pgp->b_stat, 1);
    printf("\nEquip:\n");
    equipArray_print(fp, pgp->equip, invArray);
    printf("Statistiche totali: ");
    stat_t total_stats = stat_sum(pgp->b_stat, pgp->eq_stat);
    stat_print(fp, &total_stats, 1);
    printf("\n\n");
}

void pg_updateEquip(pg_t *pgp, invArray_t invArray) {
    if(pgp == NULL || invArray == NULL)
        return;
    equipArray_update(pgp->equip, invArray);

    //Aggiorna i dati relativi alle statistiche del personaggio
    equipStats_update(pgp, invArray);
}