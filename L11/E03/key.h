#ifndef KEY_DEF
#define KEY_DEF

#define MAX_KEY 30

typedef struct key_s {
    char elab[MAX_KEY+1];
    char net[MAX_KEY+1];
} keys_t;

int key_cmp(keys_t k1, keys_t k2);

keys_t key_null();
int key_isnull(keys_t k);

void key_print(keys_t k);

keys_t key_read();

#endif