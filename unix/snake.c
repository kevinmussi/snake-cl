#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <ncurses.h>
#include <string.h>

#define ROWS 20
#define COLS 50
#define BEG_X 12
#define BEG_Y 10
#define BEG_VEL 120
#define DESTRA 1
#define SINISTRA 2
#define SU 3
#define GIU 4

typedef struct {int x; int y;} coordinate;
struct snake1 {coordinate p; struct snake1 *next;};
typedef struct snake1 snake;

int punteggio = 0;
int DIM_X, DIM_Y;
char *nick; //nickname, va passato come parametro al main
time_t start, end; //per calcolare la durata di una partita
short fine = 0;
int tipo_fine;
coordinate ostacoli[100];
coordinate cibo;

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

int main(int argc, char * argv[]) {
    snake *testa, *temp;
    int **M;
    int vel = BEG_VEL, ch, i, dir = 0; //0 = fermo, 1 = destra, 2 = sinistra, 3 = su, 4 = giù
	
    if(argc != 2) {
        printf("Error! Launch with: ./snake <nickname>\n");
        return 0;
    }
    
    nick = (char *) malloc(sizeof(char) * strlen(argv[1])); //alloca la memoria per il nickname
    strcpy(nick, argv[1]);
    testa = (snake *) malloc(sizeof(snake));
    M = (int **) malloc(ROWS * sizeof(int *));
    for(i = 0; i < ROWS; i++)
        M[i] = (int *) malloc(COLS * sizeof(int *));
    
    initscr(); //inizializzo la finestra
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
    
#define BORDO() board((DIM_Y-ROWS)/2-1,(DIM_X-COLS)/2-1,ROWS+1,COLS+2);
    
    BORDO(); //stampo il bordo
    refresh();
    
    start = time(NULL);
    
    while(fine == 0) {
        if((ch = getch()) != ERR)
            direzione(ch, &dir); //cambio la direzione
        
        if(dir != 0 && fine == 0) { //se la partita è iniziata e non si ha perso
            controlla_collisione(M, &testa, dir); //controllo se avviene una collisione
            if(fine == 0) { //se non è avvenuta una collisione
                testa = movimento(M, dir, testa); //muovo il serpente
                cambia_velocità(&vel); //aumento la velocità a seconda del punteggio
                clear(); //elimino il contenuto dello schermo
                BORDO(); //stampo il bordo
                set_colors(testa, dir);
                //test(M);
                refresh(); //aggiorno lo schermo
                usleep(vel*1000); //aspetto vel millisecondi
            }
        }
    }

#undef BORDO
    
    refresh();
    endwin(); //chiudo la finestra
    
    salva_punteggio(tipo_fine);
    
    printf(tipo_fine == 0 ? "You lost " : "You quitted ");
    printf("with %d points. Press any key to continue... ", punteggio);
    fflush(stdout);
    ch = getchar();
    
    return 0;
}

void calcola_successivo(int dir, coordinate *testa, coordinate *successivo) {
    int X, Y; //assumono solo valori -1, 0, 1, utili a ottimizzare la funzione
    
    switch(dir) {
        case DESTRA:
            X = 1;
            Y = 0;
            break;
        case SINISTRA:
            X = -1;
            Y = 0;
            break;
        case SU:
            X = 0;
            Y = -1;
            break;
        case GIU:
            X = 0;
            Y = 1;
            break;
        default: break;
    }
    
    successivo->x = testa->x + X;
    successivo->y = testa->y + Y;
}

void controlla_collisione(int **M, snake **testa, int dir) {
    coordinate successivo;
    snake *nuovo, *temp;
    int ch;
    
    calcola_successivo(dir, &((*testa)->p), &successivo);
    
    if(successivo.x >= COLS || successivo.x < 0 || successivo.y >= ROWS || successivo.y < 0) {
        tipo_fine = 0;
        fine = 1;
        return;
    }
    
#define SUCC M[successivo.y][successivo.x]
    
    if(SUCC == -1) { //se il serpente mangia il cibo
        nuovo = (snake *) malloc(sizeof(snake)); //creo un nuovo elemento del serpente
        nuovo->p.x = successivo.x; //aggiorno la posizione dell'elemento
        nuovo->p.y = successivo.y;
        nuovo->next = NULL;
        temp = *testa;
        while(temp->next != NULL) temp = temp->next;
        temp->next = nuovo;
        M[nuovo->p.y][nuovo->p.x] = 2;
        
        if(++punteggio % 5 == 0) genera(M, -2); //ogni 5 punti aggiungo un ostacolo
        genera(M, -1); //aggiungo il nuovo cibo
    } else if(SUCC == -2 || SUCC > 1) { //se è avvenuta una collisione
        tipo_fine = 0;
        fine = 1;
    }
    
#undef SUCC
    
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
        ostacoli[punteggio/5].x = x;
        ostacoli[punteggio/5].y = y;
    }
    
    return 0;
}

void direzione(int ch, int *dir) {
    int car;
    
    if(ch != ERR) {
        switch(ch) {
            case 100: //D
            case KEY_RIGHT:
                if(*dir != SINISTRA || punteggio == 0)
                    *dir = DESTRA;
                break;
            case 97: //A
            case KEY_LEFT:
                if(*dir != DESTRA || punteggio == 0)
                    *dir = SINISTRA;
                break;
            case 119: //W
            case KEY_UP:
                if(*dir != GIU || punteggio == 0)
                    *dir = SU;
                break;
            case 115: //S
            case KEY_DOWN:
                if(*dir != SU || punteggio == 0)
                    *dir = GIU;
                break;
            case 'q':
                tipo_fine = 1;
                fine = 1;
                break;
            case ' ':
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
    
    if(punteggio >= 5) { //stampo gli ostacoli
        attron(COLOR_PAIR(3));
        for(i = 0; i < punteggio/5; i++)
            ADD_CAR(ostacoli[i], ACS_DIAMOND);
        attroff(COLOR_PAIR(3));
    }
    
    attron(COLOR_PAIR(1)); //stampo il serpente
    
    switch(dir) {
        case 0:
            car = ACS_DIAMOND;
            break;
        case GIU:
            car = ACS_DARROW;
            break;
        case SU:
            car = ACS_UARROW;
            break;
        case DESTRA:
            car = ACS_RARROW;
            break;
        case SINISTRA:
            car = ACS_LARROW;
            break;
        default: break;
    }
    
    ADD_CAR(testa->p, car);
    
    if(testa->next == NULL) {
        attroff(COLOR_PAIR(1));
        return;
    }
    
    prev = testa;
    testa = testa->next;
    
    while(testa->next != NULL) {
        if(prev->p.x == (testa->next)->p.x)
            car = ACS_VLINE;
        else if(prev->p.y == (testa->next)->p.y)
            car = ACS_HLINE;
        //da qui in poi bisogna fidarsi di quello che ho fatto
        else if((prev->p.y > testa->p.y && testa->p.x < (testa->next)->p.x) ||
                (prev->p.x > testa->p.x && testa->p.y < (testa->next)->p.y))
            car = ACS_ULCORNER;
        else if((prev->p.x < testa->p.x && testa->p.y < (testa->next)->p.y) ||
                (prev->p.y > testa->p.y && testa->p.x > (testa->next)->p.x))
            car = ACS_URCORNER;
        else if((prev->p.x < testa->p.x && testa->p.y > (testa->next)->p.y) ||
                (prev->p.y < testa->p.y && testa->p.x > (testa->next)->p.x))
            car = ACS_LRCORNER;
        else if((prev->p.y < testa->p.y && testa->p.x < (testa->next)->p.x) ||
                (prev->p.x > testa->p.x && testa->p.y > (testa->next)->p.y))
            car = ACS_LLCORNER;
            
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

void cambia_velocità(int *vel) {
    if(punteggio > 0 && punteggio < 100 && punteggio % 10 == 0) {
        *vel = BEG_VEL*(100-punteggio)/100;
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
    
    fprintf(fp, "%s;%d;%d;%d;%s\n", nick, punteggio, durata, tipo_fine,
                                    ctime(&end)); //...e salvo il nuovo punteggio
    
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
