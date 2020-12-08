#ifndef INV_H
#define INV_H

#define MAX_STR 50
#define MAX_INV 8

typedef struct {
    int hp;
    int mp;
    int atk;
    int def;
    int mag;
    int spr;
} stat_t;

typedef struct {
    char name[MAX_STR+1];
    char type[MAX_STR+1];
    stat_t stat;
} inv_t;

typedef struct {
    int inUse;
    inv_t* arrEq[MAX_INV];
} tabEquip_t;

typedef struct {
    inv_t* arrInv;
    int nInv;
} tabInv_t;

int fillInventory(tabInv_t* tabinv); //Acquisizione da file (tutti gli oggetti)
inv_t readItem(FILE* fp); //Acquisizione da file (singolo oggetto)
inv_t* searchItem(char item_name[], tabInv_t* tabinv); //Cerca oggetto per nome
void printItem(inv_t item); //Stampa dati oggetto DA IMPLEMENTARE
void printStats(stat_t s); //Stampa statistiche

#endif