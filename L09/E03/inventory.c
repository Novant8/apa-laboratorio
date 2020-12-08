#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inventory.h"

#define FILE_INV "inventario.txt"

/**
 * La funzione legge i dati sugli oggetti da file e li aggiunge alla lista.
 * Return: 
 * - 1 se la lettura va a buon fine
 * - 0 se c'è stato un errore nella lettura
 */ 
int fillInventory(tabInv_t* tabinv) {
    FILE* fp = fopen(FILE_INV, "r");
    if(!fp)
        return 0;

    /* Lettura quantità oggetti */
    fscanf(fp, "%d", &tabinv->nInv);

    /* Allocazione array inventario */
    tabinv->arrInv = (inv_t*)malloc(sizeof(inv_t)*tabinv->nInv);

    /* Ciclo di lettura da file */
    for(int i=0; i<tabinv->nInv; i++)
        tabinv->arrInv[i] = readItem(fp);
    
    fclose(fp);
    
    return 1;
}

/**
 * La funzione legge i dati relativi a un oggetto da file:
 * Return:
 * - L'oggetto letto
 * - Un oggetto avente nome vuoto se la lettura non va a buon fine
 */ 
inv_t readItem(FILE* fp) {
    inv_t item;
    int res = fscanf(fp, "%s %s %d %d %d %d %d %d",
                        item.name,
                        item.type,
                        &item.stat.hp,
                        &item.stat.mp,
                        &item.stat.atk,
                        &item.stat.def,
                        &item.stat.mag,
                        &item.stat.spr);
    if(res!=8)
        item.name[0] = '\0';
    
    return item;
}

inv_t* searchItem(char item_name[], tabInv_t* tabinv) {
    for(int i=0; i<tabinv->nInv; i++)
        if(strcmp(item_name, tabinv->arrInv[i].name)==0)
            return tabinv->arrInv+i;
    return NULL;
}

void printItem(inv_t item) {
    printf("%s (tipo: %s) \t Statistiche: ", item.name, item.type);
    printStats(item.stat);
}

void printStats(stat_t s) {
    printf("{hp: %d, mp: %d, atk: %d, def: %d, mag: %d, spr: %d}\n",
                s.hp,
                s.mp,
                s.atk,
                s.def,
                s.mag,
                s.spr);
}