#include "snake.h"

// Global variables

int punteggio;
int DIM_X, DIM_Y;
char *nick;
time_t start, end;
unsigned short fine;
int tipo_fine;
int num_obst;
coordinate ostacoli[100], cibo;
int obst_freq;
boolean hard_flag;
boolean screen_wrap_flag;

// Functions

void launch(boolean no_obst, boolean hard, boolean screen_wrap) {
    snake *testa, *temp;
    int **M;
    int vel = BEG_VEL, ch, i, dir = 0; //0 = fermo, 1 = destra, 2 = sinistra, 3 = su, 4 = giù
    char car;
    
    num_obst = 0;
    punteggio = 0;
    fine = 0;
    hard_flag = hard;
    screen_wrap_flag = screen_wrap;
    
    if(no_obst == True) {
        obst_freq = 0;
    } else {
        obst_freq = (hard == True) ? 1 : 5;
    }
    
    testa = (snake *) malloc(sizeof(snake));
    M = (int **) malloc(ROWS * sizeof(int *));
    for(i = 0; i < ROWS; i++)
        M[i] = (int *) malloc(COLS * sizeof(int *));
    
    initscr(); //inizializzo la finestra
    clear();
    noecho(); //gli input non vengono mostrati a schermo
    start_color(); //uso i colori
    cbreak(); //i comandi speciali vengono annullati
    nodelay(stdscr, TRUE); //getch() non blocca l'esecuzione
    curs_set(0); //nascondo il cursore
    keypad(stdscr, TRUE); //permette di riconoscere i tasti direzionali
    
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);
    
    init(M, testa); //inizializzo la matrice
    getmaxyx(stdscr, DIM_Y, DIM_X); //ottengo le dimensioni della finestra
    set_colors(testa, 0); //stampo la situazione iniziale
    
#define BOARD() board((DIM_Y-ROWS)/2-1,(DIM_X-COLS)/2-1,ROWS+1,COLS+2);
    
    BOARD(); //stampo il bordo
    refresh();
    
    start = time(NULL);
    
    while(fine == 0) {
        if((ch = getch()) != ERR)
            direzione(ch, &dir); //cambio la direzione
        
        if(dir != 0 && fine == 0) { //se la partita è iniziata e non si ha perso
            controlla_collisione(M, &testa, dir); //controllo se avviene una collisione
            if(fine == 0) { //se non è avvenuta una collisione
                testa = movimento(M, dir, testa); //muovo il serpente
                cambia_velocita(&vel); //aumento la velocita' a seconda del punteggio
                clear(); //elimino il contenuto dello schermo
                BOARD(); //stampo il bordo
                set_colors(testa, dir);
                refresh(); //aggiorno lo schermo
                usleep(vel*1000); //aspetto vel millisecondi
            }
        }
    }

#undef BOARD
    
    clear();
    refresh();
    
    salva_punteggio(tipo_fine);
    
    printw(tipo_fine == 0 ? "You lost " : "You quit ");
    printw("with %d points. Press 'r' to restart. Press any other key to exit... ", punteggio);
    fflush(stdout);
    
    nodelay(stdscr, FALSE);
    if((car = getch()) == 'r') {
        fseek(stdin, 0, SEEK_END); // Clear input buffer
        launch(no_obst, hard, screen_wrap); // Launch a new game
    }
    
    endwin(); // Close the ncurses window
    return;
}

void calcola_successivo(int dir, coordinate *testa, coordinate *successivo) {
    int X, Y; //assumono solo valori -1, 0, 1
    
    switch(dir) {
        case RIGHT:
            X = 1;
            Y = 0;
            break;
        case LEFT:
            X = -1;
            Y = 0;
            break;
        case UP:
            X = 0;
            Y = -1;
            break;
        case DOWN:
            X = 0;
            Y = 1;
            break;
        default: break;
    }
    
    successivo->x = testa->x + X;
    successivo->y = testa->y + Y;
    
    if(screen_wrap_flag == True) {
        successivo->x = (successivo->x + COLS) % COLS;
        successivo->y = (successivo->y + ROWS) % ROWS;
    }
}

void controlla_collisione(int **M, snake **testa, int dir) {
    coordinate successivo;
    snake *nuovo, *temp;
    int ch, succ;
    
    calcola_successivo(dir, &((*testa)->p), &successivo);
    
    if(successivo.x >= COLS || successivo.x < 0 || successivo.y >= ROWS || successivo.y < 0) {
        tipo_fine = 0;
        fine = 1;
        return;
    }
    
    succ = M[successivo.y][successivo.x];
    
    if(succ == -1) { //se il serpente mangia il cibo
        nuovo = (snake *) malloc(sizeof(snake)); //creo un nuovo elemento del serpente
        nuovo->p.x = successivo.x; //aggiorno la posizione dell'elemento
        nuovo->p.y = successivo.y;
        nuovo->next = NULL;
        nuovo->direction = dir;
        temp = *testa;
        while(temp->next != NULL) temp = temp->next;
        temp->next = nuovo;
        M[nuovo->p.y][nuovo->p.x] = 2;
        punteggio++;
        if(obst_freq != 0 && punteggio % obst_freq == 0) {
            genera(M, -2); //aggiungo un ostacolo
        }
        genera(M, -1); //aggiungo il nuovo cibo
    } else if(succ == -2 || succ > 1) { //se è avvenuta una collisione
        tipo_fine = 0;
        fine = 1;
    }
}

snake *movimento(int **M, int dir, snake *testa) {
    snake *temp;
    coordinate successivo;
    
    calcola_successivo(dir, &(testa->p), &successivo);
    
    if(testa == NULL) {
        return NULL;
    } else if(punteggio == 0) {
        M[successivo.y][successivo.x] = 1; //aggiorno la matrice
        M[testa->p.y][testa->p.x] = 0;
        testa->p.x = successivo.x; //aggiorno le coordinate della testa nella lista
        testa->p.y = successivo.y;
        testa->direction = dir;
    } else {
        M[testa->p.y][testa->p.x] = 2; //cambio il valore della vecchia testa nella matrice
        temp = testa;
        while((temp->next)->next != NULL) //arrivo fino al penultimo elemento
            temp = temp->next;
        M[(temp->next)->p.y][(temp->next)->p.x] = 0;
        (temp->next)->next = testa; //la coda punta alla testa attuale
        testa = temp->next; //la coda diventa la nuova testa
        temp->next = NULL; //la nuova coda punta giustamente a NULL
        
        M[successivo.y][successivo.x] = 1; //aggiorno la testa nella matrice
        testa->p.x = successivo.x; //aggiorno le coordinate della testa nella lista
        testa->p.y = successivo.y;
        testa->direction = dir;
    }
    
    return testa;
}

int genera(int **M, int tipo) {
    int x, y;
    short valido = 0;
    
    if(tipo != -2 && tipo != -1) //caso anomalo
        return -1;
    
    srand((unsigned long) time(NULL));
    
    do {
        x = rand() % COLS; //scelta di coordinate casuali
        y = rand() % ROWS;
        
        if(M[y][x] == 0) valido = 1; //esco dal ciclo se la posizone è valida
    } while (valido == 0); //itero finché la scelta casuale non diventa valida
    
    M[y][x] = tipo; //-1 = cibo; -2 = ostacolo
    
    if(tipo == -1) {
        cibo.x = x;
        cibo.y = y;
    } else {
        if(obst_freq == 0)
            return -2;
        ostacoli[num_obst].x = x;
        ostacoli[num_obst].y = y;
        num_obst++;
    }
    
    return 0;
}

void direzione(int ch, int *dir) {
    int car;
    
    if(ch != ERR) {
        switch(ch) {
            case 100: //D
            case KEY_RIGHT:
                if(*dir != LEFT || punteggio == 0)
                    *dir = RIGHT;
                break;
            case 97: //A
            case KEY_LEFT:
                if(*dir != RIGHT || punteggio == 0)
                    *dir = LEFT;
                break;
            case 119: //W
            case KEY_UP:
                if(*dir != DOWN || punteggio == 0)
                    *dir = UP;
                break;
            case 115: //S
            case KEY_DOWN:
                if(*dir != UP || punteggio == 0)
                    *dir = DOWN;
                break;
            case 'q':
                tipo_fine = 1;
                fine = 1;
                break;
            case ' ':
            case 'p':
                nodelay(stdscr, FALSE);
                while((car = getch()) != ' '); //metto il gioco in pausa finché non si preme la barra spaziatrice
                nodelay(stdscr, TRUE); //ripristino l'impostazione di gioco
                break;
            default: break;
        }
    }
}

void set_colors(snake *testa, int dir) {
    snake *prev;
    int car, i;
    
#define OFF_X (DIM_X-COLS)/2
#define OFF_Y (DIM_Y-ROWS)/2
#define ADD_CAR(X, Y) mvaddch(X.y + OFF_Y, X.x + OFF_X, Y);
    
    attron(COLOR_PAIR(2)); //stampo il cibo
    ADD_CAR(cibo, ACS_DIAMOND);
    attroff(COLOR_PAIR(2));
    
    if(num_obst != 0) { //stampo gli ostacoli
        attron(COLOR_PAIR(3));
        for(i = 0; i < num_obst; i++)
            ADD_CAR(ostacoli[i], ACS_DIAMOND);
        attroff(COLOR_PAIR(3));
    }
    
    attron(COLOR_PAIR(1)); //stampo il serpente
    
    switch(dir) {
        case 0:
            car = ACS_DIAMOND;
            break;
        case DOWN:
            car = ACS_DARROW;
            break;
        case UP:
            car = ACS_UARROW;
            break;
        case RIGHT:
            car = ACS_RARROW;
            break;
        case LEFT:
            car = ACS_LARROW;
            break;
        default: break;
    }
    
    ADD_CAR(testa->p, car);
    
    if(testa->next == NULL) {
        attroff(COLOR_PAIR(1));
        return;
    }
    
    // We traverse the snake from its head to its tail, so pr
    prev = testa;
    testa = testa->next;
    while(testa->next != NULL) {
        if(prev->p.x == (testa->next)->p.x)
            car = ACS_VLINE;
        else if(prev->p.y == (testa->next)->p.y)
            car = ACS_HLINE;
        else if((testa->direction == RIGHT && prev->direction == UP) ||
                (testa->direction == DOWN && prev->direction == LEFT))
            car = ACS_LRCORNER;
        else if((testa->direction == LEFT && prev->direction == UP) ||
                (testa->direction == DOWN && prev->direction == RIGHT))
            car = ACS_LLCORNER;
        else if((testa->direction == RIGHT && prev->direction == DOWN) ||
                (testa->direction == UP && prev->direction == LEFT))
            car = ACS_URCORNER;
        else if((testa->direction == LEFT && prev->direction == DOWN) ||
                (testa->direction == UP && prev->direction == RIGHT))
            car = ACS_ULCORNER;
            
        ADD_CAR(testa->p, car);
        prev = testa;
        testa = testa->next;
    }
    
    car = (prev->p.x == (prev->next)->p.x) ? ACS_VLINE : ACS_HLINE;
    
    ADD_CAR(testa->p, car);
    
    attroff(COLOR_PAIR(1));
    
#undef OFF_X
#undef OFF_Y
#undef ADDCAR
    
}

void cambia_velocita(int *vel) {
    if(hard_flag == True) {
        *vel = 48;
    } else if(punteggio > 0 && punteggio < 90 && punteggio % 10 == 0) {
        *vel = BEG_VEL - punteggio*4/5;
    }
}

void init(int **M, snake *testa) {
    register int i, j;
    
    for(i = 0; i < ROWS; i++)
    	for(j = 0; j < COLS; j++)
    		M[i][j] = 0; //azzero la matrice
    
    M[BEG_Y][BEG_X] = 1; //inizializzo il punto di partenza
    M[BEG_Y][(BEG_X+COLS)/2] = -1; //inizializzo il cibo
    testa->p.x = BEG_X; //salvo le coordinate di partenza nella struct
    testa->p.y = BEG_Y;
    testa->next = NULL;
    testa->direction = 0;
    cibo.x = (BEG_X+COLS)/2;
    cibo.y = BEG_Y;
}

int salva_punteggio(int tipo_fine) {
    FILE *fp;
    int durata;
    char ch;
    
    end = time(NULL);
    durata = (int) difftime(end, start); //durata della partita
    
    if((fp = fopen("snake_log.txt", "at")) == NULL) //apro il file
        return -1;
        
    while ((ch = fgetc(fp)) != EOF); //arrivo alla fine del file...
    
    char *time = ctime(&end);
    char *p = strchr(time, '\n');
    if(p) *p = '\0';
    
    fprintf(fp, "%s;%d;%d;%d\n", time, punteggio, durata, tipo_fine); //...e salvo il nuovo punteggio
    
    fclose(fp); //chiudo il file
    
    return 0;
}

void board(int starty, int startx, int lines, int cols) {
    int endy, endx, i;
    
    endy = starty + lines;
    endx = startx + cols;
    
    for(i = startx; i <= endx; i++)
        mvaddch(starty, i, ACS_HLINE);
    for(i = startx; i <= endx; i++)
        mvaddch(endy, i, ACS_HLINE);
    for(i = starty; i <= endy; i++)
        mvaddch(i, startx, ACS_VLINE);
    for(i = starty; i <= endy; i++)
        mvaddch(i, endx, ACS_VLINE);
    
    mvaddch(starty, startx, ACS_ULCORNER);
    mvaddch(endy, startx, ACS_LLCORNER);
    mvaddch(starty, endx, ACS_URCORNER);
    mvaddch(endy, endx, ACS_LRCORNER);
}

void test(int **M) {
    int i, j;
    
    for(i = 0; i < ROWS; i++) {
        printw("\n");
        for(j = 0; j < COLS; j++) printw("%2d", M[i][j]);
    }
    
    refresh();
}
