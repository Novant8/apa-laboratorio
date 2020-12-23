#include <stdio.h>
#include <stdlib.h>

#define BOARD_FILE "board.txt"
#define TILES_FILE "tiles.txt"

typedef struct {
    char color;
    int val;
} tube_t;

typedef struct {
    tube_t h;
    tube_t v;
} tile_t;

typedef struct {
    int t_i; //indice tessera
    int r; //rotazione
} btile_t; //board_tile

tile_t* getTiles(int* n_tiles);
btile_t** getBoard(tile_t* tiles, int* mark, int* R, int* C);
void getMax(tile_t* tiles, btile_t** board, int* mark, int n_tiles, int R, int C);
void getMax_R(int r, int c, tile_t* tiles, btile_t** board, int* mark, btile_t** board_max, int* max_score, int n_tiles, int R, int C);
int getScore(btile_t** board, tile_t* tiles, int R, int C);
void free2d(btile_t** m, int R);
void printBoard(btile_t** board, tile_t* tiles, int R, int C);

int main() {

    int n_tiles,R,C;
    tile_t* tiles = getTiles(&n_tiles);
    int* mark = (int*)calloc(n_tiles, sizeof(int));
    btile_t** board = getBoard(tiles, mark, &R, &C);
    if(tiles == NULL || board == NULL) {
        printf("Errore in lettura file");
        return -1;
    }

    getMax(tiles, board, mark, n_tiles, R, C);

    free(tiles);
    free(mark);
    free2d(board, R);

    return 0;
}

/**
 * La funzione legge da file i dati di tutte le tessere inseribili
 */ 
tile_t* getTiles(int* n_tiles) {
    FILE* fp = fopen(TILES_FILE, "r");
    if(!fp)
        return NULL;

    fscanf(fp, "%d\n", n_tiles);

    tile_t* tiles = (tile_t*)malloc(*n_tiles*sizeof(tile_t));
    for(int i=0; i<*n_tiles; i++)
        fscanf(fp, "%c %d %c %d\n", &tiles[i].h.color, &tiles[i].h.val, &tiles[i].v.color, &tiles[i].v.val);

    return tiles;
}

/**
 * La funzione genera la board iniziale, leggendo da file.
 * Tutte le tessere lette vengono marcate come utilizzate (valore 1 nel vettore mark), in modo da non poterle inserire due volte durante l'esplorazione delle soluzioni.
 */ 
btile_t** getBoard(tile_t* tiles, int* mark, int* R, int* C) {
    FILE* fp = fopen(BOARD_FILE, "r");
    if(!fp)
        return NULL;

    fscanf(fp, "%d %d", R, C);

    tube_t tmp;
    int t_i, r;
    btile_t** board = (btile_t**)malloc(*R*sizeof(btile_t*));
    for(int i=0; i<*R; i++) {
        board[i] = (btile_t*)malloc(*C*sizeof(btile_t));
        for(int j=0; j<*C; j++) {
            fscanf(fp, "%d/%d", &board[i][j].t_i, &board[i][j].r);
            //Marca la tessera come utilizzata se la piastrella corrente non è vuota
            if(board[i][j].t_i != -1)
                mark[board[i][j].t_i] = 1;
        }
    }

    return board;
}

/**
 * Funzione wrapper che stampa la configurazione avente punteggio massimo
 */ 
void getMax(tile_t* tiles, btile_t** board, int* mark, int n_tiles, int R, int C) {
    //board_max alloc
    btile_t** board_max = (btile_t**)malloc(R*sizeof(btile_t*));
    for(int i=0; i<R; i++)
        board_max[i] = (btile_t*)malloc(C*sizeof(btile_t));

    printf("Scacchiera iniziale:\n");
    printBoard(board, tiles, R, C);

    int max_score = 0;
    getMax_R(0, 0, tiles, board, mark, board_max, &max_score, n_tiles, R, C);

    printf("Configurazione massimo punteggio (max_score = %d):\n", max_score);
    printBoard(board_max, tiles, R, C);

    free2d(board_max, R);
}

/**
 * Funzione che esplora ricorsivamente tutte le possibili configurazioni (usando il modello delle disposizioni semplici) e salva in board_max quella massima.
 */
void getMax_R(int r, int c, tile_t* tiles, btile_t** board, int* mark, btile_t** board_max, int* max_score, int n_tiles, int R, int C) {
    if(r>=R) {
        c++;
        r=0;
    }
    if(c>=C) {
        int score = getScore(board, tiles, R, C);
        if(score > *max_score) {
            *max_score = score;
            for(r=0; r<R; r++)
                for(c=0; c<C; c++)
                    board_max[r][c] = board[r][c];
        }
        return;
    }
    tube_t tmp;
    if(board[r][c].t_i == -1) {
        for(int i=0; i<n_tiles; i++)
            if(mark[i] == 0) {
                mark[i] = 1;
                //Tessera non ruotata
                board[r][c].t_i = i;
                board[r][c].r = 0;
                getMax_R(r+1, c, tiles, board, mark, board_max, max_score, n_tiles, R, C);
                //Tessera ruotata
                board[r][c].r = 1;
                getMax_R(r+1, c, tiles, board, mark, board_max, max_score, n_tiles, R, C);
                //Backtrack
                board[r][c].t_i = -1;
                mark[i] = 0;
            }
    } else {
        getMax_R(r+1, c, tiles, board, mark, board_max, max_score, n_tiles, R, C);
    }
}

/**
 * La funzione restituisce il punteggio totale di una board passata per parametro
 */
int getScore(btile_t** board, tile_t* tiles, int R, int C) {
    int i, j, score_tmp, score=0; /* score immagazzina il punteggio totale, score_tmp tiene conto del punteggio temporaneamente durante la scannerizzazione delle piastrelle */
    tube_t current, prev;
    tile_t first;

    //CALCOLO PUNTEGGIO PER RIGHE
    for(i=0; i<R; i++) {
        //Inizializza score_tmp al valore della casella alla prima colonna
        score_tmp = board[i][0].r ? tiles[board[i][0].t_i].v.val : tiles[board[i][0].t_i].h.val;
        //Calcola il punteggio di ogni cella
        for(j=1; j<C; j++) {
            current = board[i][j].r ? tiles[board[i][j].t_i].v : tiles[board[i][j].t_i].h;
            prev = board[i][j-1].r ? tiles[board[i][j-1].t_i].v : tiles[board[i][j-1].t_i].h;
            //Se i colori della cella corrente e quella precedente sono diversi, allora il punteggio della riga corrente è nullo
            if(current.color != prev.color) {
                score_tmp = 0;
                break;
            }
            score_tmp+=current.val;
        }
        score+=score_tmp;
    }

    //CALCOLO PUNTEGGIO PER COLONNE
    for(j=0; j<C; j++) {
        //Inizializza score_tmp al valore della casella alla prima riga
        score_tmp = board[0][j].r ? tiles[board[0][j].t_i].h.val : tiles[board[0][j].t_i].v.val;
        //Calcola il punteggio di ogni cella
        for(i=1; i<R; i++) {
            current = board[i][j].r ? tiles[board[i][j].t_i].h : tiles[board[i][j].t_i].v;
            prev = board[i-1][j].r ? tiles[board[i-1][j].t_i].h : tiles[board[i-1][j].t_i].v;
            //Se il colore della cella corrente e quello della precedente sono diversi, allora il punteggio della colonna corrente è nullo
            if(current.color != prev.color) {
                score_tmp = 0;
                break;
            }
            score_tmp+=current.val;
        }
        score+=score_tmp;
    }

    return score;
}

void free2d(btile_t** m, int R) {
    for(int i=0; i<R; i++)
        free(m[i]);
    free(m);
}

void printBoard(btile_t** board, tile_t* tiles, int R, int C) {
    tile_t t;
    tube_t tmp;
    for(int i=0; i<R; i++) {
        for(int j=0; j<C; j++) {
            if(board[i][j].t_i != -1) {
                t = tiles[board[i][j].t_i];
                if(board[i][j].r) {
                    tmp = t.h;
                    t.h = t.v;
                    t.v = tmp;
                }
                printf("[h: (%c, %d), v: (%c, %d)]\t", t.h.color, t.h.val, t.v.color, t.v.val);
            } else {
                printf("[vuota]\t");
            }
        }
        printf("\n");
    }
}