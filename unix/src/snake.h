#ifndef SNAKE_H
#define SNAKE_H

// Libraries

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <ncurses.h>
#include <string.h>

// Macros

#define ROWS 20
#define COLS 50
#define BEG_X 12
#define BEG_Y 10
#define BEG_VEL 120
#define RIGHT 1
#define LEFT 2
#define UP 3
#define DOWN 4

// Typedefs

typedef struct {
    int x;
    int y;
} coordinate;

typedef struct snake {
    coordinate p;
    struct snake *next;
    int direction;
} snake;

typedef enum boolean {
    False = 0,
    True = 1
} boolean;

// Functions

void launch();
void init(int **, snake *);
void set_colors(snake *, int);
void direzione(int, int *);
int salva_punteggio(int);
void controlla_collisione(int **, snake **, int);
void cambia_velocità(int *);
snake *movimento(int **, int, snake *);
int genera(int **, int);
void calcola_successivo(int, coordinate *, coordinate *);
void board(int, int, int, int);
void test(int **);

#endif
