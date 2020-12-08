#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inventory.h"
#include "character.h"

#define MAX_CMD 24

typedef enum {
    c_add_ch,
    c_search_ch,
    c_del_ch,
    c_add_equipment,
    c_rmv_equipment,
    c_search_item,
    c_end,
    c_unknown
} cmd_e;

cmd_e readcmd();
void handlecmd(cmd_e cmd, tabCh_t* tabch, tabInv_t* tabinv);

int main() {

    tabCh_t* tabch = (tabCh_t*)malloc(sizeof(tabCh_t));
    tabch->headCh = tabch->tailCh = NULL;
    tabInv_t* tabinv = (tabInv_t*)malloc(sizeof(tabInv_t));
    if(!readCh_all(tabch)) {
        printf("Errore durante la lettura del file dei personaggi.\n");
        return -1;
    }
    if(!fillInventory(tabinv)) {
        printf("Errore durante la lettura del file dell'inventario.\n");
        return -1;
    }

    cmd_e cmd;
    do {
        cmd = readcmd();
        handlecmd(cmd, tabch, tabinv);
    } while(cmd != c_end);

    //FREE
    free_ch(tabch->headCh);
    free(tabch);
    free(tabinv->arrInv);
    free(tabinv);

    return 0;
}

cmd_e readcmd() {
    char* cmds[c_unknown] = {"aggiungi_personaggio", "cerca_personaggio", "elimina_personaggio", "aggiungi_equipaggiamento", "rimuovi_equipaggiamento", "cerca_oggetto", "fine"};
    char cmd_str[MAX_CMD+1];
    printf("Inserire comando: ");
    scanf("%s", cmd_str);

    cmd_e cmd;
    for(cmd=c_add_ch; cmd < c_unknown && strcmp(cmds[cmd], cmd_str); cmd++);

    return cmd;
}

void handlecmd(cmd_e cmd, tabCh_t* tabch, tabInv_t* tabinv) {
    ch_t *ch;
    ch_t ch_i;
    inv_t* item;
    int cod;
    char item_name[MAX_STR+1];
    switch(cmd) {
        case c_add_ch:
            printf("Inserire i dati del personaggio: ");
            getchar(); /* Istruzione per catturare carattere extra */
            ch_i = readCh(stdin);
            if(ch_i.cod > 0)
                addCh_list(ch_i, tabch);
            else
                printf("Dati inseriti invalidi.\n");
            break;
        case c_search_ch:
            printf("Inserire codice personaggio: ");
            scanf("%d", &cod);
            ch = searchCh(cod, tabch->headCh);
            if(ch != NULL)
                printChStats(ch);
            else
                printf("Personaggio non trovato.\n");
            break;
        case c_del_ch:
            printf("Inserire codice personaggio: ");
            scanf("%d", &cod);
            if(delCh_list(cod, tabch))
                printf("Personaggio cancellato!\n");
            else
                printf("Nessun personaggio trovato.\n");
            break;
        case c_add_equipment:
        case c_rmv_equipment:
            printf("Inserire codice personaggio: ");
            scanf("%d", &cod);
            printf("Inserire nome oggetto: ");
            scanf("%s", item_name);

            item = searchItem(item_name, tabinv);
            ch = searchCh(cod, tabch->headCh);
            if(item != NULL)
                if(cmd == c_add_equipment)
                    equipItem(item, ch);
                else
                    removeItem(item, ch);
            else
                printf("Dati inseriti non validi.\n");
            break;
        case c_search_item:
            printf("Inserire nome oggetto: ");
            scanf("%s", item_name);
            item = searchItem(item_name, tabinv);
            if(item != NULL)
                printItem(*item);
            else
                printf("Oggetto non trovato.\n");
            break;
        case c_end:
            break;
        case c_unknown:
            printf("Comando sconosciuto.\n");
    }
}