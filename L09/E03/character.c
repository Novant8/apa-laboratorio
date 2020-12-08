#include <stdio.h>
#include <stdlib.h>
#include "character.h"

#define FILE_CH "pg.txt"

stat_t getStats(ch_t* ch);
stat_t statsSum(stat_t s1, stat_t s2);
void printEquipment(ch_t ch);
void printCh(ch_t ch);
nodeCh_t* new_nodeCh(ch_t ch, nodeCh_t* next);

/**
 * La funzione legge i dati dei personaggi da file e li aggiunge alla lista.
 * Return: 
 * - 1 se la lettura va a buon fine
 * - 0 se c'è stato un errore nella lettura
 */ 
int readCh_all(tabCh_t* tabch) {
    FILE* fp = fopen(FILE_CH, "r");
    if(!fp)
        return 0;

    ch_t ch_tmp;
    while((ch_tmp = readCh(fp)).cod!=-1) { //Se è stato letto un personaggio valido
        addCh_list(ch_tmp, tabch);
        tabch->nCh++;
    }

    fclose(fp);

    return 1;
}

/**
 * La funzione aggiunge un personaggio alla lista IN CODA
 */ 
void addCh_list(ch_t ch, tabCh_t* tabch) {
    if(tabch->headCh == NULL) {
        tabch->headCh = tabch->tailCh = new_nodeCh(ch, NULL);
    } else {
        nodeCh_t* new = new_nodeCh(ch, NULL);
        tabch->tailCh->next = new;
        tabch->tailCh = new;
    }
}

nodeCh_t* new_nodeCh(ch_t ch, nodeCh_t* next) {
    nodeCh_t* new = (nodeCh_t*)malloc(sizeof(nodeCh_t));
    new->ch = ch;
    new->next = next;
    
    return new;
}

/**
 * La funzione rimuove un personaggio identificato dal codice cod dalla lista
 * Return:
 * - 1 se il personaggio è stato cancellato
 * - 0 se non è stato trovato un personaggio avente codice cod
 */ 
int delCh_list(int cod, tabCh_t* tabch)  {
    nodeCh_t *n, *tmp;
    for(n=tabch->headCh; n->next != NULL; n=n->next)
        if(n->next->ch.cod == cod) {
            tmp = n->next;
            n->next = n->next->next;
            free(tmp);
            return 1;
        }
    return 0;
}

stat_t statsSum(stat_t s1, stat_t s2) {
    stat_t sum;
    sum.hp = s1.hp + s2.hp;
    sum.mp = s1.mp + s2.mp;
    sum.atk = s1.atk + s2.atk;
    sum.def = s1.def + s2.def;
    sum.mag = s1.mag + s2.mag;
    sum.spr = s1.spr + s2.spr;
    return sum;
}

stat_t getStats(ch_t* ch) {
    stat_t s = ch->stat;

    /* Primo ciclo che somma tutte le statistiche */
    for(int i=0; i<ch->equip->inUse; i++)
        s = statsSum(s, ch->equip->arrEq[i]->stat);
    
    /* Controllo che tutte le statistiche siano positive o nulle. In caso contrario, vengono impostate a 0 */
    if(s.hp<0) s.hp=0;
    if(s.mp<0) s.mp=0;
    if(s.atk<0) s.atk=0;
    if(s.def<0) s.def=0;
    if(s.mag<0) s.mag=0;
    if(s.spr<0) s.spr=0;
    
    return s;
}

void printChStats(ch_t* ch) {
    stat_t stats = getStats(ch);
    printf("PERSONAGGIO:\n");
    printCh(*ch);
    printf("EQUIPAGGIAMENTO:\n");
    printEquipment(*ch);
    printf("STATISTICHE TOTALI: ");
    printStats(stats);
}

void printCh(ch_t ch) {
    printf("PG%04d %s %s %d %d %d %d %d %d\n",
                ch.cod,
                ch.name,
                ch.class,
                ch.stat.hp,
                ch.stat.mp,
                ch.stat.atk,
                ch.stat.def,
                ch.stat.mag,
                ch.stat.spr);
}

void printEquipment(ch_t ch) {
    for(int i=0; i<ch.equip->inUse; i++) {
        printf("%d. ", i+1);
        printItem(*ch.equip->arrEq[i]);
    }
}

/**
 * La funzione legge i dati relativi a un personaggio da file:
 * Return:
 * - Il personaggio letto
 * - Un personaggio avente codice -1 se la lettura non va a buon fine
 */ 
ch_t readCh(FILE* fp) {
    ch_t ch;
    int res = fscanf(fp, "PG%d %s %s %d %d %d %d %d %d\n",
                        &ch.cod,
                        ch.name,
                        ch.class,
                        &ch.stat.hp,
                        &ch.stat.mp,
                        &ch.stat.atk,
                        &ch.stat.def,
                        &ch.stat.mag,
                        &ch.stat.spr);
    if(res!=9) {
        ch.cod=-1;
    } else {
        ch.equip = (tabEquip_t*)malloc(sizeof(tabEquip_t));
        ch.equip->inUse = 0;
    }
    
    return ch;
}

/**
 * La funzione aggiunge l'oggetto puntato da item nell'inventario del personaggio puntato da ch
 */ 
int equipItem(inv_t* item, ch_t* ch) {
    int n_items = ch->equip->inUse;
    if(n_items >= MAX_INV)
        return 0;
    ch->equip->arrEq[n_items] = item;
    ch->equip->inUse++;
    return 1;
}

/**
 * La funzione rimuove l'oggetto puntato da item nell'inventario del personaggio puntato da ch
 */ 
int removeItem(inv_t* item, ch_t* ch) {
    int n_items = ch->equip->inUse;
    if(n_items >= MAX_INV)
        return 0;
    
    int i,j;
    /* Ricerca dell'indice dell'array contenente il puntatore item */
    for(int i=0; i<n_items; i++)
        if(ch->equip->arrEq+i == item) {
            /* Shift a sinistra di uno dell'array equipaggiamento, solo per gli indici maggiori di quello da rimuovere */
            for(j=i; j<n_items-1; j++)
                ch->equip->arrEq[i] = ch->equip->arrEq[i+1];
            break;
        }
    ch->equip->inUse--;
    return 1;
}

void free_ch(nodeCh_t* head) {
    if(head == NULL)
        return;
    free_ch(head->next);
    free(head->ch.equip);
    free(head);
}

ch_t* searchCh(int cod, nodeCh_t* head) {
    if(head == NULL)
        return NULL;
    if(head->ch.cod == cod)
        return &head->ch;
    return searchCh(cod, head->next);
}