#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>

#define CELL '#'
#define EMPTY '.'

int **create(int sizeX, int sizeY);
int input(int **array, int sizeX, int sizeY);
void output(int **array, int sizeX, int sizeY);
int array_creator(int ***array, int ***clonedArray, int *sizeX, int *sizeY, int *time);
void game_of_life(int **field, int **oldField, int sizeX, int sizeY, int time);
int life(int **oldField, int y, int x, int sizeX, int sizeY);

int main() {
    int **inputArray = NULL;
    int **clonedArray = NULL;
    int time;
    int sizeX, sizeY;
    int creatorCode = array_creator(&inputArray, &clonedArray, &sizeX, &sizeY, &time);
    if (creatorCode == 1) {
        game_of_life(inputArray, clonedArray, sizeX, sizeY, time);
        free(inputArray);
        free(clonedArray);
    }
    return 0;
}

int **create(int sizeX, int sizeY) {
    int **array = malloc(sizeX * sizeY * sizeof(int) + sizeY * sizeof(int *));
    if (NULL != array) {
        int *ptr = (int *)(sizeY + array);
        for (int i = 0; i < sizeY; i++) {
            array[i] = ptr + i * sizeX;
        }
    }
    return array;
}

int input(int **array, int sizeX, int sizeY) {
    int returnCode = 1;
    for (int i = 0; i < sizeY; i++) {
        for (int j = 0; j < sizeX; j++) {
            if (returnCode == 1) {
                if (scanf("%i", &array[i][j]) != 1 || array[i][j] > 1 || array[i][j] < 0) {
                    returnCode = 0;
                }
            }
        }
    }
    return returnCode;
}

void output(int **array, int sizeX, int sizeY) {
    for (int i = 0; i < sizeY; i++) {
        for (int j = 0; j < sizeX; j++) {
            if (j != 0) {
                printf(" ");
            }
            if (array[i][j] == 1) {
                printf("%c", CELL);
            } else {
                printf("%c", EMPTY);
            }
        }
        printf("\n");
    }
}

int array_creator(int ***array, int ***clonedArray, int *sizeX, int *sizeY, int *time) {
    int resultCode = 1;
    printf("Enter X, Y and game speed (time in ms per frame):\n");
    if (scanf("%i %i %i", sizeX, sizeY, time) != 3 || *sizeX <= 0 || *sizeY <= 0 || *time <= 0) {
        printf("Index is not int or below 0\n");
        resultCode = 0;
    } else {
        *array = create(*sizeX, *sizeY);
        *clonedArray = create(*sizeX, *sizeY);
        if (NULL == *array || NULL == *clonedArray) {
            printf("Can't allocate memory!\n");
            if (NULL != *array) {
                free(*array);
            }
            if (NULL != *clonedArray) {
                free(*clonedArray);
            }
            resultCode = 0;
        } else {
            if (input(*array, *sizeX, *sizeY) == 0) {
                printf("Error in input()\n");
                free(*array);
                free(*clonedArray);
                resultCode = -1;
            }
        }
    }
    return resultCode;
}

void game_of_life(int **field, int **oldField, int sizeX, int sizeY, int time) {
    printf("\e[1;1H\e[2J");
    int endCounter = -1;
    output(field, sizeX, sizeY);
    usleep(time);
    while (1) {
        for (int i = 0; i < sizeY; i++) {
            for (int j = 0; j < sizeX; j++) {
                if (oldField[i][j] != field[i][j] && endCounter != -1) {
                    endCounter += 1;
                }
                oldField[i][j] = field[i][j];
            }
        }
        if (endCounter == 0) {
            break;
        }
        for (int i = 0; i < sizeY; i++) {
            for (int j = 0; j < sizeX; j++) {
                field[i][j] = life(oldField, i, j, sizeX, sizeY);
            }
        }
        printf("\e[1;1H\e[2J");
        output(field, sizeX, sizeY);
        endCounter = 0;
        usleep(time);
    }
    printf("It was wonderful! End of game of life!");
}

int life(int **oldField, int y, int x, int sizeX, int sizeY) {
    int result = oldField[y][x];
    int counter = 0;
    for (int i = y - 1; i <= y + 1; i++) {
        for (int j = x - 1; j <= x + 1; j++) {
            int tempJ;
            int tempI;
            if (j == -1) {
                tempJ = sizeX - 1;
            } else {
                tempJ = j;
            }
            if (i == -1) {
                tempI = sizeY - 1;
            } else {
                tempI = i;
            }
            if (oldField[tempI % sizeY][tempJ % sizeX] == 1 && !(i == y && j == x)) {
                counter++;
            }
        }
    }
    if (counter == 3 || (counter <= 3 && counter >= 2 && result == 1)) {
        result = 1;
    } else {
        result = 0;
    }
    return result;
}
