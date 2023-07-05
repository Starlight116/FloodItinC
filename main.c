#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SIZE 12
#define COLORS 6
#define TRYS 22

#define FG_SHIFT 30
#define BG_SHIFT 40

typedef enum { BLACK = 0, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE, MAX_COLORS } ColorCode;

ColorCode** createGrid(int);
void destroyGrid(ColorCode***, int);
void printColor(int, int, char);
void printGrid(ColorCode**, int);
void randomGrid(ColorCode**, int, int);
int isFlooded(ColorCode**, int);
void propagate(ColorCode**, int, ColorCode);
void propagateRec(ColorCode**, int, ColorCode, ColorCode, int, int);
void play(int, int, int);

ColorCode** createGrid(int t) {
    ColorCode** grid = malloc(t * sizeof(ColorCode*));
    for (int i = 0; i < t; i++) {
        grid[i] = malloc(t * sizeof(ColorCode));
    }
    for (int j = 0; j < t; j++) {
        for (int k = 0; k < t; k++) { grid[j][k] = BLACK; }
    }
    return grid;
}

void destroyGrid(ColorCode*** grid, int t){
    for (int i = 0; i < t; i++) {
        free(grid[i]);
    }
    free(grid);
}

void printColor(int fg, int bg, char c)
{
    printf("\033[%d;%dm %c\033[m", FG_SHIFT + fg, BG_SHIFT + bg, c);
}

void printGrid(ColorCode** grid, int t) {
    for (int i = 0; i < t; i++) {
        for (int j = 0; j < t; j++) {
            printColor(WHITE, BLACK + grid[i][j], '0' + grid[i][j]);
        }
        printf("\n");
    }
}

void randomGrid(ColorCode** grid, int t, int nbCol) {
    srand(time(NULL));
    for (int i = 0; i < t; i++) {
        for (int j = 0; j < t; j++) {
            grid[i][j] = (rand() % nbCol - 1) + 2;
        }
    }
}

int isFlooded(ColorCode** grid, int t) {
    ColorCode c = grid[0][0];
    int flooded = -1;
    for (int i = 0; i < t; i++) {
        for (int j = 0; j < t; j++) {
            if (grid[i][j] != c) {
                flooded = 0;
            }
            else flooded = 1;
        }
    }
    return flooded;
}

void propagate(ColorCode** grid, int t, ColorCode col) {
    if (col != grid[0][0]) propagateRec(grid, t, grid[0][0], col, 0, 0);
}
void propagateRec(ColorCode** G, int t, ColorCode old_c, ColorCode new_c, int i, int j) {
    G[i][j] = new_c;

    /* left */
    if ((j>0) && (G[i][j-1] == old_c))
        propagateRec(G, t, old_c, new_c, i, j-1);

    /* down */
    if ((i<t-1) && (G[i+1][j] == old_c))
        propagateRec(G, t, old_c, new_c, i+1, j);

    /* right */
    if ((j<t-1) && (G[i][j+1] == old_c))
        propagateRec(G, t, old_c, new_c, i, j+1);

    /* up */
    if ((i>0) && (G[i-1][j] == old_c))
        propagateRec(G, t, old_c, new_c, i-1, j);
}


void play(int t, int nb_col, int n)
{
    ColorCode** G;
    int res = -1;

    // initialisation
    G = createGrid(t);
    randomGrid(G, t, nb_col);
    printGrid(G, t);

    for (int i = 0; i < n; ++i)
    {
        printf("YOU STILL HAVE %d TRYS !\n", n - i);
        printf("TYPE A NUMBER BETWEEN [1..6] (-1 TO QUIT) : ");
        scanf("%d", &res);
        if (res == -1) break;
        ColorCode col = res;
        propagate(G, t, col);
        printGrid(G, t);
    }

    if (res == -1){
        printf("BYE!\n");
    }
    else if (isFlooded(G, t)) {
        printf("WIN!\n");
    }
    else {
        printf("LOST!\n");
    }
    destroyGrid(&G, t);

}

int main() {
    play(SIZE,COLORS,TRYS);
    return EXIT_SUCCESS;
}
