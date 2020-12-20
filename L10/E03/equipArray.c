#include <stdio.h>
#include <stdlib.h>

#include "equipArray.h"

struct equipArray_s {
    int items[EQUIP_SLOT];
    int inUse;
};

equipArray_t equipArray_init() {
    equipArray_t equipArray = (equipArray_t)malloc(sizeof(struct equipArray_s));
    return equipArray;
}

int equipArray_inUse(equipArray_t equipArray) {
    return equipArray->inUse;
}

void equipArray_print(FILE *fp, equipArray_t equipArray, invArray_t invArray) {
    for(int i=0; i<equipArray->inUse; i++)
        inv_print(fp, invArray_getByIndex(invArray, equipArray->items[i]));
}

void equipArray_free(equipArray_t equipArray) {
    free(equipArray);
}

void equipArray_update(equipArray_t equipArray, invArray_t invArray) {
    char op[2], itemName[LEN];
    printf("Vuoi aggiungere o rimuovere oggetti (r/a)? ");
    scanf("%s", op);
    op[1] = '\0';

    printf("Inserire nome oggetto: ");
    scanf("%s", itemName);
    int item_idx = invArray_searchByName(invArray, itemName);

    if(item_idx < 0) {
        printf("Oggetto non trovato.\n");
        return;
    }

    switch(op[0]) {
        case 'r':
            if(equipArray_remove(equipArray, invArray, item_idx))
                printf("Oggetto rimosso!\n");
            else
                printf("Oggetto non presente nell'equipaggiamento!\n");
            break;
        case 'a':
            if(equipArray_add(equipArray, invArray, item_idx))
                printf("Oggetto aggiunto!\n");
            else
                printf("Equipaggiamento pieno!\n");
            break;
        default:
            printf("Operazione invalida.\n");
    }
}

int equipArray_remove(equipArray_t equipArray, invArray_t invArray, int index) {
    int tmp;
    //Sposta l'oggetto da rimuovere al fondo
    for(int i=0; i<equipArray->inUse-1; i++)
        if(equipArray->items[i] == index) {
            tmp = equipArray->items[i];
            equipArray->items[i] = equipArray->items[equipArray->inUse-1];
            equipArray->items[equipArray->inUse-1] = tmp;
            equipArray->inUse--;
            return 1;
        }
    return 0;
}

int equipArray_add(equipArray_t equipArray, invArray_t invArray, int index) {
    if(equipArray->inUse >= EQUIP_SLOT)
        return 0;
    equipArray->items[equipArray->inUse++] = index;
    return 1;
}

int equipArray_getEquipByIndex(equipArray_t equipArray, int index) {
    return equipArray->items[index];
}