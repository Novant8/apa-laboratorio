#include <stdio.h>
#include <stdlib.h>

int majority(int *a, int N);
int count(int toCount, int *a, int N);
int arrayContains(int search, int *a, int N);

int main(int argc, char** argv) {

    if(argc < 2) {
        printf("USO CORRETTO: %s <n1> [n_2,[n_3,...]]\n", *argv);
        return -1;
    }

    const int N = argc-1;
    unsigned int vet[N];

    for(int i=1; i<argc; i++)
        vet[i-1] = atoi(argv[i]);

    int maj = majority(vet, N);
    if(maj>=0)
        printf("Il numero maggioritario è %d\n", maj);
    else
        printf("Non è presente un numero maggioritario nel vettore.\n");
}

int majority(int *a, int N) {
    if(N==1)
        return *a;

    int newN = N/2;
    int maj_sx = majority(a, newN);
    int maj_dx = majority(a+newN, N-newN);
    int n_sx=0, n_dx=0;

    if(maj_sx == maj_dx)
        return maj_sx;

    if(maj_sx != -1) {
        n_sx = count(maj_sx, a, N);
        if(n_sx > newN)
            return maj_sx;
    }
    
    if(maj_dx != -1) {
        n_dx = count(maj_dx, a, N);
        if(n_dx > newN)
            return maj_dx;
    }

    return -1;
}

int count(int toCount, int *a, int N) {
    int cnt=0;
    for(int i=0; i<N; i++)
        if(a[i]==toCount)
            cnt++;
    return cnt;
}
