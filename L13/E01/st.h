#ifndef ST_H
#define ST_H

#define MAX_KEY 30

typedef struct st_s *st_t;

/* Inizializzazione/Distruzione */
st_t st_init(int n_items);
void st_free(st_t st);

/* Aggiunta */
int st_add(st_t st, char* id);

/* Estrazione */
char* st_getIdByIndex(st_t st, int i);
int st_getIndexById(st_t st, char* id);

#endif