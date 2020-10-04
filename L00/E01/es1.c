#include <stdio.h>

#define N_MAX 30

typedef struct {
    int pos;
    int length;
}Sottovettore;

void stampaSottovettore(int[], int);

int main()  {

    int i=0;
    int v[N_MAX];
    printf("Inserire una serie di numeri interi maggiori o uguali a 0. Inserire un numero negativo per terminare l'inserimento.\n");
    do {
        scanf("%d", v+i);
        i++;
    } while(i<N_MAX-1 && v[i-1]>=0);

    int max = 0;
    int n_sv = 0; //NUMERO DI SOTTOVETTORI
    int l_sv = 0; //LUNGHEZZA SOTTOVETTORE
    Sottovettore sv[N_MAX/2];
    printf("Vettore: [");
    for(i=0; i<N_MAX && v[i] >= 0; i++) {
        printf(" %d", v[i]);
        if(v[i] == 0) {
            sv[n_sv].length = l_sv;
            if(l_sv > max)
                max = l_sv;
            l_sv = 0;
            n_sv++;
            if(i+1 < N_MAX && v[i+1]>=0)
                sv[n_sv].pos = i+1;
        } else {
            l_sv++;
        }
    }
    printf("]\n");

    printf("Vettori di lunghezza massima (%d):", max);
    for(i=0; i<n_sv; i++)
        if(sv[i].length == max)
            stampaSottovettore(v, sv[i].pos);
    printf('\n');

    return 0;
}

void stampaSottovettore(int v[], int pos) {
    printf("[");
    for(int i=pos; i<N_MAX && v[i]>0; i++)
        printf(" %d", v[i]);
    printf(" ], ");
}