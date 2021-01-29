#ifndef ST_H
#define ST_H

typedef struct st_s *ST;

/* Inizializzazione e liberazione dalla memoria */
ST STinit(int max_keys);
void STfree(ST st);

/* Aggiunta alla tabella */
void STadd(ST st, char* key);

/* Estrazione di indice e chiave */
int STgetIndex(ST st, char* key);
void STgetKey(ST st, int idx, char* key);

#endif