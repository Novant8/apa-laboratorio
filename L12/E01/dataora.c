#include "dataora.h"

data_t data_read(FILE* fp) {
    data_t d;
    if(fscanf(fp, "%d/%d/%d", &d.a, &d.m, &d.g) != 3)
        d = data_null();
    return d;
}

ora_t ora_read(FILE* fp) {
    ora_t h;
    if(fscanf(fp, "%d:%d", &h.h, &h.m) != 3)
        h = ora_null();
    return h;
}

static int data_toint(data_t d) {
    return d.a * 10000 + d.m*100 + d.g;
}

int data_cmp(data_t d1, data_t d2) {
    return data_toint(d1)-data_toint(d2);
}

static int ora_toint(ora_t h) {
    return h.h*100 + h.m;
}

int ora_cmp(ora_t h1, ora_t h2) {
    return ora_toint(h1)-ora_toint(h2);
}

void data_print(FILE* fp, data_t d) {
    if(data_cmp(d, data_null())!=0)
        fprintf(fp, "%d/%02d/%02d", d.a, d.m, d.g);
    else
        fprintf(fp, "DATA INVALIDA");
}

void ora_print(FILE* fp, ora_t h) {
    if(ora_cmp(h, ora_null())!=0)
        fprintf(fp, "%d:%02d", h.h, h.m);
    else
        fprintf(fp, "ORA INVALIDA");
}

data_t data_null() {
    data_t d;
    d.a = d.m = d.g = -1;
    return d;
}

ora_t ora_null() {
    ora_t h;
    h.h = h.m = -1;
    return h;
}

int data_isnull(data_t d) {
    return data_cmp(d, data_null()) == 0;
}

int ora_isnull(ora_t h) {
    return ora_cmp(h, ora_null()) == 0;
}