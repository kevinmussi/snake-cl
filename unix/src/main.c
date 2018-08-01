#include "snake.h"

#define VERSION "1.0"

int main(int argc, char * argv[]) {
    char *par;
    
    // Default flags
    boolean hard = False;
    boolean no_obstacles = False;
    boolean screen_wrap = False;
    
    // Evaluate the program parameters
    int i;
    int len;
    for(i = 1; i < argc; i++) {
        par = argv[i];
        len = strlen(par);
        
        if((len == 6 && !strcmp(par, "--hard")) ||
               (len == 2 && !strcmp(par, "-H"))) {
            hard = True;
        } else if((len == 13 && !strcmp(par, "--screen-wrap")) ||
                      (len == 3 && !strcmp(par, "-sw"))) {
            screen_wrap = True;
        } else if((len == 14 && !strcmp(par, "--no-obstacles")) ||
                      (len == 4 && !strcmp(par, "-nob"))) {
            no_obstacles = True;
        } else if((len == 6 && !strcmp(par, "--help")) ||
                      (len == 2 && !strcmp(par, "-h"))) {
            printf("OVERVIEW: Command-line based Snake game. Made by: Kevin Ludovico Mussi.\n\n");
            printf("URL: https://github.com/kevinmussi/snake-cl\n\n");
            printf("USAGE: snake-cl [flags]\n\nFLAGS:\n  ");
            printf("-h, --help\t\tPrint this help message.\n  ");
            printf("-v, --version\t\tPrint the game's version.\n  ");
            printf("-H, --hard\t\tPlay the hard mode (this overrides the \"--no-obstacles\" flag).\n  ");
            printf("-sw, --screen-wrap\tPlay with the screen wrap mode.\n  ");
            printf("-nob, --no-obstacles\tPlay without the generation of obstacles.\n");
            return 0;
        } else if((len == 9 && !strcmp(par, "--version")) ||
                      (len == 2 && !strcmp(par, "-v"))) {
            printf(VERSION);
            printf("\n");
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
