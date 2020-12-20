#include <stdio.h>
#include <stdlib.h>

#include "pg.h"

static stat_t empty_stats() {
    stat_t s;
    s.atk = s.def = s.hp = s.mag = s.mp = s.spr = 0;
    return s;
}

static stat_t stat_sum(stat_t s1, stat_t s2)  {
    stat_t s;
    s.atk = s1.atk + s2.atk;
    s.def = s1.def + s2.def;
    s.hp = s1.hp + s2.hp;
    s.mag = s1.mag + s2.mag;
    s.mp = s1.mp + s2.mp;
    s.spr = s1.spr + s2.spr;
    return s;
}

static void equipStats_update(pg_t* pgp, invArray_t invArray) {
    stat_t eq_stat = empty_stats();
    int inv_idx;
    inv_t* item;

    for(int i=0; i<equipArray_inUse(pgp->equip); i++) {
        inv_idx = equipArray_getEquipByIndex(pgp->equip, i);
        item = invArray_getByIndex(invArray, inv_idx);
        eq_stat = stat_sum(eq_stat, item->stat);
    }
}

int pg_read(FILE *fp, pg_t *pgp) {
    int res = fscanf(fp, "%s %s %s", pgp->cod, pgp->nome, pgp->classe);
    res = res == 3 && stat_read(fp, &pgp->b_stat);
    return res;
}

void pg_clean(pg_t *pgp) {
    equipArray_free(pgp->equip);
}

void pg_print(FILE *fp, pg_t *pgp, invArray_t invArray) {
    fprintf(fp, "%s %s %s ", pgp->cod, pgp->nome, pgp->classe);
    stat_print(fp, &pgp->b_stat, 9999);
    printf("\nEquip:\n");
    equipArray_print(fp, pgp->equip, invArray);
    printf("\n");
}

void pg_updateEquip(pg_t *pgp, invArray_t invArray) {
    equipArray_update(pgp->equip, invArray);
    equipStats_update(pgp, invArray);
}