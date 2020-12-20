#include <stdio.h>
#include <stdlib.h>

#include "inv.h"

static int real_stat(int val, int soglia);

int stat_read(FILE *fp, stat_t *statp) {
    int res = fscanf(fp, "%d %d %d %d %d %d",
                &statp->hp,
                &statp->mp,
                &statp->atk,
                &statp->def,
                &statp->mag,
                &statp->spr);
    return res == 6;
}

void stat_print(FILE *fp, stat_t *statp, int soglia) {
    stat_t to_print;
    to_print.hp = real_stat(statp->hp, soglia);
    to_print.mp = real_stat(statp->mp, soglia);
    to_print.atk = real_stat(statp->atk, soglia);
    to_print.def = real_stat(statp->def, soglia);
    to_print.mag = real_stat(statp->mag, soglia);
    to_print.spr = real_stat(statp->spr, soglia);

    fprintf(fp, "%d %d %d %d %d %d",
                to_print.hp,
                to_print.mp,
                to_print.atk,
                to_print.def,
                to_print.mag,
                to_print.spr);
}

void inv_read(FILE *fp, inv_t *invp) {
    fscanf(fp, "%s %s", invp->nome, invp->tipo);
    stat_read(fp, &invp->stat);
    printf("\n");
}

void inv_print(FILE *fp, inv_t *invp) {
    fprintf(fp, "%s %s ", invp->nome, invp->tipo);
    stat_print(fp, &invp->stat, 9999);
    printf("\n");
}

stat_t inv_getStat(inv_t *invp) {
    return invp->stat;
}

/* Dato il valore di una statistica, restituisce il valore effettivo da stampare */
static int real_stat(int val, int soglia) {
    if(val < 0)
        return 1;
    if(val > soglia)
        return soglia;
    return val;
}