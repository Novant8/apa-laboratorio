#include <stdio.h>
#include <stdlib.h>

#include "inv.h"

/**
 * La funzione, data una struttura di tipo stat_t, restituisce un'altra struttura dello stesso tipo non contenente statistiche negative. 
 */ 
static stat_t real_stats(stat_t statp) {
    if(statp.hp <= 0) statp.hp = 1;
    if(statp.mp <= 0) statp.mp = 1;
    if(statp.atk <= 0) statp.atk = 1;
    if(statp.def <= 0) statp.def = 1;
    if(statp.mag <= 0) statp.mag = 1;
    if(statp.spr <= 0) statp.spr = 1;
    return statp;
}

/**
 * Date due strutture stat_t, la funzione restituisce la somma delle loro statistiche
 */ 
stat_t stat_sum(stat_t s1, stat_t s2)  {
    stat_t s;
    s.atk = s1.atk + s2.atk;
    s.def = s1.def + s2.def;
    s.hp = s1.hp + s2.hp;
    s.mag = s1.mag + s2.mag;
    s.mp = s1.mp + s2.mp;
    s.spr = s1.spr + s2.spr;
    return s;
}

/**
 * La funzione genera una struttura di tipo stat_t vuota
 */ 
stat_t empty_stats() {
    stat_t s;
    s.atk = s.def = s.hp = s.mag = s.mp = s.spr = 0;
    return s;
}

int stat_read(FILE *fp, stat_t *statp) {
    if(statp == NULL)
        return 0;
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
    if(statp == NULL) {
        printf("Statistiche invalide.\n");
        return;
    }

    stat_t to_print;
    
    //In base al valore della booleana soglia, vengono stampate statistiche negative o  no
    if(soglia)
        to_print = real_stats(*statp);
    else
        to_print = *statp;

    fprintf(fp, "%d %d %d %d %d %d",
                to_print.hp,
                to_print.mp,
                to_print.atk,
                to_print.def,
                to_print.mag,
                to_print.spr);
}

void inv_read(FILE *fp, inv_t *invp) {
    if(invp == NULL)
        return;
    fscanf(fp, "%s %s", invp->nome, invp->tipo);
    stat_read(fp, &invp->stat);
    printf("\n");
}

void inv_print(FILE *fp, inv_t *invp) {
    if(invp == NULL) {
        printf("Oggetto invalido\n");
        return;
    }
    fprintf(fp, "%s %s ", invp->nome, invp->tipo);
    stat_print(fp, &invp->stat, 0);
    printf("\n");
}

stat_t inv_getStat(inv_t *invp) {
    if(invp == NULL)
        return empty_stats();
    return invp->stat;
}