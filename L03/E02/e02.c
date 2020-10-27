#include <stdio.h>
#include <string.h>

int isBigEndian();
void stampaCodifica(void* p, int size, int bigEndian);
void aggiungiByte(int binary[], unsigned char* p_c, int currentByte);
void stampaBit(int binary[], int n);
void convertiBinario(int binary[], unsigned char* p_c, int size, int bigEndian);
int getPadding(int bigEndian, int size);

int main() {

    float af;
    double ad;
    long double ald;

    /* Individua ordine dei byte */
    int bigEndian = isBigEndian();
    int size;
    printf("La macchina usa una modalità d'accesso del tipo %s.\n\n", bigEndian ? "big endian" : "little endian");

    /*printf("Il long double contiene %d bit di padding\n\n", getPadding(bigEndian, sizeof(long double)));*/

    /* Input numero decimale */
    printf("Inserire un numero decimale: ");
    scanf("%f", &af);

    /* Lo stesso numero viene immagazzinato nei dati di tipo double e long double */
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

/**
 * La funzione verifica se l'ordine dei byte è del tipo big endian o little endian
 * Return:
 * - 0 se è little endian
 * - 1 se è big endian
 */ 
int isBigEndian() {
    unsigned int x = 1;     /* Immagazzina in 4 byte di memoria un 1 intero senza segno (i tre byte più significativi saranno tutti 0, il quarto avrà tutti zeri e un 1 come LSB) */
    char *c = (char*) &x;   /* Casta l'intero in un puntatore di char, così questo punterà al primo byte in memoria */
    return *c != 1;         /* Se il byte puntato da c è 1 allora è il byte meno significativo (little endian), altrimenti è quello più significativo (big endian) */
}

void stampaCodifica(void* p, int size, int bigEndian) {
    int n_bit = size*8;
    unsigned char* p_c = (unsigned char*)p;
    int padding = size > 8 ? getPadding(bigEndian, size) : 0; /* Se la codifica è in long double vengono calcolati i bit di padding */
    int n_bits = size*8;
    int binary[n_bits]; /* Bit salvati dal più significativo (idx 0) al meno significativo (idx n_bits-1) */

    convertiBinario(binary, p_c, size, bigEndian);

    /* In base al tipo di dato (identificato da sizeof), viene identificato il numero di bit dell'esponente  */
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
            printf("Codifica dato sconosciuta.\n");
            return;
    }
    
    /* Output numero binario */
    printf("Bit segno: %d\n", *(binary+padding));
    printf("Bit esponente: ");
    stampaBit(binary+padding+1, n_exp);
    printf("Bit mantissa: ");
    stampaBit(binary+padding+n_exp+1, n_bits-padding-n_exp-1);
    
    printf("\n");
}

/**
 * La funzione converte un numero in binario, e lo salva nell'array binary[].
 */ 
void convertiBinario(int binary[], unsigned char* p_c, int size, int bigEndian) {
    memset(binary, 0, size*8*sizeof(int)); /* Inizializza tutti i bit a 0 */
    int i;
    /* In base all'ordine dei byte, la conversione comicia dal primo o dall'ultimo byte in memoria*/
    if(bigEndian)
        for(i=0; i<size; i++)
            aggiungiByte(binary, p_c+i, i);
    else
        for(i=size-1; i>=0; i--)
            aggiungiByte(binary, p_c+i, size-1-i);
}

/**
 * La funzione converte il byte puntato da p_c in binario e lo aggiunge all'array binary[].
 * L'indice currentByte indica la posizione del byte relativa all'array binary e comincia dal byte più significativo.
 */
void aggiungiByte(int binary[], unsigned char* p_c, int currentByte) {
    int p_i = (int)*p_c;        /* Variabile temporanea che contiene il byte codificato in intero */
    int i=(currentByte+1)*8;    /* L'indice comincia dalla fine del byte */

    /* Conversione in binario mediante divisioni successive per due. */
    while(p_i>0) {
        binary[--i] = p_i%2;
        p_i/=2;
    }
}

/**
 * La funzione stampa i primi n bit contenuti in binary[] a schermo
 */ 
void stampaBit(int binary[], int n) {
    for(int i=0; i<n; i++)
        printf("%d", binary[i]);
    printf("\n");
}

/**
 * La funzione ricava il numero di bit di padding del long double
 */ 
int getPadding(int bigEndian, int size) {
    long double x = 1;
    long double y = -1;

    int n_bits = size * 8;

    int x_bin[n_bits]; /* Bit salvati dal più significativo (idx 0) al meno significativo (idx n_bits-1) */
    int y_bin[n_bits];

    /* Converti 1 e -1 in binario */
    convertiBinario(x_bin, (unsigned char*)&x, size, bigEndian);
    convertiBinario(y_bin, (unsigned char*)&y, size, bigEndian);

    /* Leggi gli array x_bin e y_bin al contrario finchè il bit all'i-esima posizione è diverso: quello è il bit di segno */
    int i;
    for(i=n_bits-1; x_bin[i] == y_bin[i]; i--);

    return i;
}