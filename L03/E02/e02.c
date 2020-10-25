#include <stdio.h>
#include <stdlib.h>

#define LD_PADDING 6

int isBigEndian();
void stampaCodifica(void* p, int size, int bigEndian);
void aggiungiByte(char binary[], unsigned char* p_c, int currentByte);
void stampaBit(char binary[], int n);

int main() {

    float af;
    double ad;
    long double ald;

    int bigEndian = isBigEndian();
    int size;
    printf("La macchina usa una modalità d'accesso del tipo %s.\n\n", bigEndian ? "big endian" : "little endian");

    printf("Inserire un numero decimale: ");
    scanf("%f", &af);

    ad = af;
    ald = af;

    size = sizeof(float);
    printf("\nCodifica float (dimensione %d byte):\n", size);
    stampaCodifica((void*)&af, size, bigEndian);

    size = sizeof(double);
    printf("Codifica double (dimensione %d byte):\n", size);
    stampaCodifica((void*)&ad, size, bigEndian);

    size = sizeof(long double);
    printf("Codifica long double (dimensione %d byte):\n", size);
    stampaCodifica((void*)&ald, size, bigEndian);

    return 0;
}

int isBigEndian() {
    int x = 1;
    char *c = (char*) &x;
    return *c != 1;
}

void stampaCodifica(void* p, int size, int bigEndian) {
    int n_bit = size*8, i;
    unsigned char* p_c = (unsigned char*)p;
    int padding = size > 8 ? LD_PADDING : 0;
    int n_bits = (size-padding)*8;
    char binary[n_bits+1];
    
    if(bigEndian)
        for(i=0; i<size; i++)
            aggiungiByte(binary, p_c+i, i);
    else
        for(i=size-1-padding; i>=0; i--)
            aggiungiByte(binary, p_c+i, size-1-padding-i);
    binary[(size-padding)*8] = '\0';

    int n_exp;
    switch(size){
        case sizeof(float):
            n_exp = 8;
        break;
        case sizeof(double):
            n_exp = 11;
        break;
        case sizeof(long double):
            n_exp = 15;
        break;
        default:
            printf("Codifica dato invalida.\n");
            return;
    }
    
    printf("Bit segno: %c\n", binary[0]);
    printf("Bit esponente: ");
    stampaBit(binary+1, n_exp);
    printf("Bit mantissa: ");
    stampaBit(binary+n_exp+1, (size-padding)*8-n_exp-1);
    
    /*for(i=0; i<(size-padding)*8; i++) {
        printf("%c", binary[i]);
        if(i>0 && i%8==0)
            printf(" ");
    }*/
    printf("\n");
}

void aggiungiByte(char binary[], unsigned char* p_c, int currentByte) {
    int p_i = (int)*p_c;
    int i=currentByte*8;
    int pow = 128;

    while(pow>0)  {
        if(p_i>=pow) {
            binary[i] = '1';
            p_i-=pow;
        } else {
            binary[i] = '0';
        }
        i++;
        pow/=2;
    }
}

void stampaBit(char binary[], int n) {
    for(int i=0; i<n; i++)
        printf("%c", binary[i]);
    printf("\n");
}