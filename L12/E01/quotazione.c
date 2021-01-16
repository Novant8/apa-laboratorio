#include "quotazione.h"

trans_t transazione_leggi(FILE* fp) {
    trans_t tr;
    tr.data = data_read(fp);
    tr.ora = ora_read(fp);
    if(fscanf(fp, "%f %d", &tr.val, &tr.num) != 2)
        tr=transazione_null();
    return tr;
}

int transazione_cmp(trans_t tr1, trans_t tr2) {
    return  data_cmp(tr1.data, tr2.data) == 0 &&
            tr1.val == tr2.val &&
            tr1.num == tr2.num;
}

trans_t transazione_null() {
    trans_t tr;
    tr.data = data_null();
    tr.val = -1;
    tr.num = -1;
    return tr;
}

void quotazione_print(FILE* fp, quot_t q) {
    fprintf(fp, "[ Data: ");
    data_print(fp, q.data);
    fprintf(fp, " Quotazione: %f ]", q.somma/q.num);
}