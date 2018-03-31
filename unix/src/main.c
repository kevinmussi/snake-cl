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
            printf("OVERVIEW: Command-line based Snake game. Made by: Kevin Ludovico Mussi.\n\n");
            printf("URL: https://github.com/kevinmussi/snake-cl\n\n");
            printf("USAGE: snake [flags]\n\nFLAGS:\n  ");
            printf("-h, --help\t\tPrint this help message.\n  ");
            printf("--hard\t\tPlay the hard mode.\n  ");
            printf("-sw, --screen-wrap\tPlay with the screen wrap mode.\n  ");
            printf("-no, --no-obstacles\tPlay without the generation of obstacles.\n");
            return 0;
        } else {
            printf("Error! Unrecognized flag %s!\n", par);
            return -1;
        }
    }
    
    // Launch the game
    launch(no_obstacles, hard, screen_wrap);
    
    return 0;
}
