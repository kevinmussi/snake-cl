#include "snake.h"

int main(int argc, char * argv[]) {
    if(argc != 2) {
        printf("Error! Launch with: ./snake <nickname>\n");
        return 0;
    }
    
    //nick = (char *) malloc(sizeof(char) * strlen(argv[1])); //alloca la memoria per il nickname
    //strcpy(nick, argv[1]);
    
    launch(False, False, True);
    
    return 0;
}
