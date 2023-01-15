#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define size 5
#define bomb 5

int board[size][size];
int bombs[size * size - 1];
int mask[size][size];

int countBomb(int openDest[2]) {
    int i, j, x, y;
    int count = 0;
    for(i = -1; i <= 1; i++) {
        for(j = -1; j <= 1; j++) {
            x = openDest[0] + i;
            y = openDest[1] + j;
            if(x == 0 && y == 0) continue;
            if((x < 0 || x >= size) || (y < 0 || y >= size)) continue;
            if(board[x][y] == -1) count++;
        }
    }
    return count;
}

int cleared() {
    int i, j;
    int count = 0;
    for(i = 0; i < size; i++) {
        for(j = 0; j < size; j++) {
            if(mask[i][j] == 0) count++;
        }
    }
    return count == size * size - bomb;
}

void printBoard() {
    int i, j;
    for(i = 0; i < size; i++) {
        for(j = 0; j < size; j++) {
            if(mask[i][j] == 1) {
                printf("# ");
                continue;
            } else if (mask[i][j] == -1) {
                printf("F ");
                continue;
            }
            switch(board[i][j]) {
                case -1:
                    printf("* ", board[i][j]);
                    break;
                case 0:
                    printf("- ", board[i][j]);
                    break;
                default:
                    printf("%d ", board[i][j]);
                    break;
            }
        }
        printf("\n\n");
    }
    return;
}

void chainOpen(int startFrom[2]) {
    printf("(%d,%d):%d\n", startFrom[0], startFrom[1], board[startFrom[0]][startFrom[1]]);
    if(board[startFrom[0]][startFrom[1]] != 0) return;
    int i, j, dest[2];
    for(i = -1; i <= 1; i++) {
        for(j = -1; j <= 1; j++) {
            dest[0] = startFrom[0] + i;
            dest[1] = startFrom[1] + j;
            if(i == 0 && j == 0) continue;
            if((dest[0] < 0 || dest[0] >= size) || (dest[1] < 0 || dest[1] >= size)) continue;
            if(board[dest[0]][dest[1]] != -1 && mask[dest[0]][dest[1]] == 1) {
                mask[dest[0]][dest[1]] = 0;
                chainOpen(dest);
            }
        }
    }
}

int main(void) {
    srand((int)time(NULL));

    int i, j;

    int openDest[2] = {0, 0};

    // set bomb and shuffle
    for(i = 0; i < size * size -1; i++) {
        if (i < bomb) {
            bombs[i] = -1;
        }
        else {
            bombs[i] = 0;
        }
    }
    for(i = 0; i < size * size -1; i++) {
        int j = rand() % (size * size -1);
        int t = bombs[i];
        bombs[i] = bombs[j];
        bombs[j] = t;
    }

    // create mask
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            mask[i][j] = 1;
        }
    }

    // ask first open dest
    printf("GAME START\n");
    printf("where? (y, x)\n");
    scanf("%d %d", &openDest[0], &openDest[1]);

    // set board avoid first bomb
    for(i = 0; i < size * size; i++) {
        int x = i % size;
        int y = i / size;
        if(x == openDest[0] && y == openDest[1]) {
            board[x][y] = 0;
            i++;
            x++;
        }
        board[x][y] = bombs[i];
    }

    // countup bombs and map count
    for(i = 0; i < size; i++) {
        for(j = 0; j < size; j++) {
            if(board[i][j] == 0) {
                int dest[2] = {i, j};
                board[i][j] = countBomb(dest);
            }
        }
    }

    mask[openDest[0]][openDest[1]] = 0;
    if (board[openDest[0]][openDest[1]] == 0) {
        printf("chain open\n");
        chainOpen(openDest);
    }

    printf("\n\n");
    printBoard(board, mask);

    int mode = 0;
    while(1) {
        printf("mode? (open:0/flag:1)\n");
        scanf("%d", &mode);
        printf("where? (y, x)\n");
        scanf("%d %d", &openDest[0], &openDest[1]);
        mask[openDest[0]][openDest[1]] = mode == 0 
                                            ? 0
                                            : mask[openDest[0]][openDest[1]] == -1
                                                ? 1
                                                : -1;
        printf("\n\n");
        if(board[openDest[0]][openDest[1]] == 0 && mode == 0) {
            printf("chain open\n");
            chainOpen(openDest);
        }
        printBoard(board, mask);
        if(board[openDest[0]][openDest[1]] == -1 && mode == 0) {
            printf("GAME OVER\n\n");
            break;
        }
        if(cleared()) {
            printf("GAME CLEAR!\n");
            break;
        }
    }
}
