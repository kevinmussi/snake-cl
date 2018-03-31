#include "snake.h"

int main(int argc, char * argv[]) {
    char *par;
    
    // Default flags
    boolean hard = False;
    boolean no_obstacles = False;
    boolean screen_wrap = False;
    
    // Evaluate the program parameters
    int i;
    for(i = 1; i < argc; i++) {
        par = argv[i];
        
        if(!strcmp(par, "--hard")) {
            hard = True;
        } else if(!strcmp(par, "--screen-wrap") || !strcmp(par, "-sw")) {
            screen_wrap = True;
        } else if(!strcmp(par, "--no-obstacles") || !strcmp(par, "-no")) {
            no_obstacles = True;
        } else if(!strcmp(par, "--help") || !strcmp(par, "-h")) {
            printf("Usage: snake [flags]\nFlags:\n\t");
            printf("-h: print this help message.\n\t");
            printf("-hard: play the hard mode.\n\t");
            printf("-screen-wrap, -sw: play with the screen wrap mode.\n\t");
            printf("-no-obstacles, -no: play without the generation of obstacles.\n\t");
        } else {
            printf("Error! Unrecognized flag %s!\n", par);
            return -1;
        }
    }
    
    // Launch the game
    launch(no_obstacles, hard, screen_wrap);
    
    return 0;
}
