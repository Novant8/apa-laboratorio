#ifndef CHAR_H
#define CHAR_H

#include <stdio.h>
#include "inventory.h"

#define MAX_STR 50

typedef struct {
    int cod;
    char name[MAX_STR+1];
    char class[MAX_STR+1];
    tabEquip_t* equip;
    stat_t stat;
} ch_t;

typedef struct node_ch {
    ch_t ch;
    struct node_ch* next;
} nodeCh_t;

typedef struct {
    nodeCh_t* headCh;
    nodeCh_t* tailCh;
    int nCh;
} tabCh_t;

int readCh_all(tabCh_t* tabch); //Acquisizione da file (più personaggi)
void addCh_list(ch_t ch, tabCh_t* tabch); //Inserimento
int delCh_list(int cod, tabCh_t* tabch); //Eliminazione
void printChStats(ch_t* ch); //Stampa dettagli
ch_t readCh(FILE* fp); //Acquisizione da file (un personaggio)
//void printCh(ch_t ch);
int equipItem(inv_t* item, ch_t* ch); //Aggiunta a equipaggiamento
int removeItem(inv_t* item, ch_t* ch); //Rimozione da equipaggiamento
//void printEquipment(ch_t ch);
ch_t* searchCh(int cod, nodeCh_t* head); //Ricerca personaggio per codice
void free_ch(nodeCh_t* head); //Deallocazione da memoria

#endif