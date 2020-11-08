#include <stdio.h>
#include <stdlib.h>

int gcd(int a, int b);

int main(int argc, char* argv[]) {
    
    if(argc < 3) {
        printf("USO CORRETTO: %s <a> <b>\n", *argv);
        return -1;
    }

    int a = atoi(argv[1]),
        b = atoi(argv[2]);

    printf("Il massimo comun divisore tra %d e %d è %d.\n", a, b, gcd(a,b));

    return 0;
}

int gcd(int a, int b) {
    if(a==b)
        return a;
    if(b>a)
        return gcd(b,a);

    int a_odd=a%2;
    int b_odd=b%2;
    if(a_odd && b_odd)
        return gcd((a-b)/2, b);
    if(a_odd)
        return gcd(a,b/2);
    if(b_odd)
        return gcd(a/2,b);
    return 2*gcd(a/2,b/2);
}