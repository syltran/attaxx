#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <MLV/MLV_all.h>
#define TAILLE_PLATEAU 7
#define TAILLE_MAX_NOM 20
#define TAILLE_CASE 60
#define MARGE 50

typedef struct {
    char nom[TAILLE_MAX_NOM];
    char symbol;
    int score;
} Joueur; 

typedef struct {
    char plateau[TAILLE_PLATEAU + 2][TAILLE_PLATEAU + 2];
    Joueur *joueurs[2];
} Plateau;



/* INITIALISATION */

/**
* BUT : determine si le nom saisie par le joueur est valide
* autrement dit s'il fait moins de 20 caracteres
* PARAMETRE : une chaine de caracteres
* RETOUR : 1 si la chaine fait moins de 20 caracteres
*          0 sinon
*/
int nomValide(char nom[]){
    int i = 0;
    while (nom[i] != '\0'){
        i++;
    }
    if (i < 20){
        return 1;
    }
    return 0;
}

/**
* BUT :
* initialise un joueur en lui associant un nom, un symbol et un score
* PARAMETRES :
*   un pointeur sur la structure Joueur
*   une chaine de caracteres rang : le rang du joueur (premier ou deuxieme)
*   une chaine de caracteres symbol : le symbol du joueur ("o" ou "x")
*/
void initJoueur(Joueur *j, char rang[], char symbol){
    int i = 0;
    char nom[TAILLE_MAX_NOM];
    do{
        printf("Quel est le nom du %s joueur (symbol %c) : ", rang, symbol);
        scanf("%s", nom);
    } while(nomValide(nom) != 1);

    while (nom[i] != '\0'){
        j->nom[i] = nom[i];
        i++;
    }    
    j->nom[i] = '\0';
    j->symbol = symbol;
    j->score = 2;
}

/**
* BUT : initialise l'ordi en lui associant un nom, un symbol et un score
* PARAMETRES :
*   un pointeur sur la structure Joueur
*   une chaine de caracteres symbol : le symbol de l'ordi ("o" ou "x")
*/
void initOrdi(Joueur *ordi, char symbol){
    char nom[] = "BOT";
    int i = 0;
    while(nom[i] != '\0'){
        ordi->nom[i] = nom[i];
        i++;
    }
    ordi->nom[i] = '\0';
    ordi->symbol = symbol;
    ordi->score = 2;
}

/**
* BUT : initialise le plateau de jeu
* PARAMETRES :
*   un pointeur sur la structure Plateau
*   deux pointeurs sur la structure Joueur
*/
void initPlateau(Plateau *p, Joueur *j1, Joueur *j2){
    p->joueurs[0] = &(*j1);
    p->joueurs[1] = &(*j2);

    int i, j;
    for (i = 0; i<TAILLE_PLATEAU+2; i++){
        for (j = 0; j<TAILLE_PLATEAU+2; j++){
            if (i == 0 || i == TAILLE_PLATEAU+1 || j == 0 || j == TAILLE_PLATEAU+1){
                p->plateau[i][j] = '*';
            }
            else if ((i == 1 && j == 1) || (i == TAILLE_PLATEAU && j == TAILLE_PLATEAU)){
                p->plateau[i][j] = 'x';
            }
            else if ((i == 1 && j == TAILLE_PLATEAU) || (i == TAILLE_PLATEAU && j == 1)){
                p->plateau[i][j] = 'o';
            }
            else{
                p->plateau[i][j] = '.';
            }
        }
    }
}

/**
* BUT : affiche le plateau
* PARAMETRE : une structure Plateau
*/
void affPlateau(Plateau p){
    int i, j;
    for (i = 0; i<TAILLE_PLATEAU+2; i++){
        for (j = 0; j<TAILLE_PLATEAU+2; j++){
            printf("%c ", p.plateau[i][j]);
        }
        if (i <= TAILLE_PLATEAU){
            printf("\n");
        }
    }
    printf("\n");
}




/* POSAGE DES PIONS SUR LE PLATEAU */

/**
* BUT : determine si le coup joué est valide
* PARAMETRES :
*   une structure Plateau
*   une structure Joueur : le joueur adverse
*   deux entiers x et y : les coordonnées de la case ou
*   le joueur souhaite poser un pion
* RETOUR : 1 si le coup est valide
*          0 sinon
*/
int coupValide(Plateau p, Joueur j2, int x, int y){
    int i, j;
    /*Si le pion est dans le plateau*/
    if((x >= 1 && x <= TAILLE_PLATEAU) && (y >= 1 && y <= TAILLE_PLATEAU)){
        /*Si la case est vide*/
        if (p.plateau[x][y] == '.'){
            /*Si le pion est à une distance 1 ou 2 d'un pion adverse*/
            for (i = -2; i<3; i++){
                for (j = -2; j<3; j++){
                    if (p.plateau[x+i][y+j] == j2.symbol){
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

/**
* BUT : modifie le score d'un joueur
* PARAMETRES :
*   un pointeur sur la structure Plateau
*   une structure Joueur : le joueur dont on veut modifier le score
*   un entier nb : 1 si on veut augmenter son score
*                  -1 si on veut baisser son score
*/
void modifScore(Plateau *p, Joueur j, int nb){
    if (j.symbol == 'o'){
        p->joueurs[0]->score += nb;
    }
    else{
        p->joueurs[1]->score += nb;
    }
}

/**
* BUT :
* change la couleur des pions adverses se situant sur les cases voisines
* de la case du pion posé
* PARAMETRES :
*   un pointeur sur la structure Plateau
*   deux structures Joueur
*   deux entiers x et y : les coordonnées de la case ou le pion a été posé
*/
void changeColor(Plateau *p, Joueur j1, Joueur j2, int x, int y){
    p->plateau[x][y] = j1.symbol;
    modifScore(&(*p), j1, 1);

    int i, j;
    for (i = -1; i<2; i++){
        for (j = -1; j<2; j++){
            if (p->plateau[x+i][y+j] == j2.symbol){
                p->plateau[x+i][y+j] = j1.symbol;
                modifScore(&(*p), j1, 1);
                modifScore(&(*p), j2, -1);
            }
        }
    }
}

/**
* BUT : 
* gere la pose d'un pion sur le plateau via le terminal
* PARAMETRES :
*   un pointeur sur la structure Plateau
*   deux structures Joueur
*/
void posePion(Plateau *p, Joueur j1, Joueur j2){
    int x, y; /*coordonnées du pion à poser*/
    do{
        printf("%s (%c), veuillez saisir les coordonnées où jouer (entre 1 et %d) : ", j1.nom, j1.symbol, TAILLE_PLATEAU);
        scanf("%d %d", &x, &y);
    } while (coupValide(*p, j2, x, y) != 1); /*Tant que le coup n'est pas valide*/
    
    changeColor(&(*p), j1, j2, x, y);
}

/**
* BUT : gere la pose des pions de l'ordi sur le plateau
* PARAMETRES :
*   un pointeur sur la structure Plateau
*   deux structures Joueur
*/
void posePionOrdi(Plateau *p, Joueur ordi, Joueur j1){
    int xRandom, yRandom;
    do {
        xRandom = rand() % TAILLE_PLATEAU + 1;
        yRandom = rand() % TAILLE_PLATEAU + 1;
    } while (coupValide(*p, j1, xRandom, yRandom) != 1);

    changeColor(&(*p), ordi, j1, xRandom, yRandom);
}




/* AFFICHAGE DES SCORES */

/**
* BUT : affiche le score courant
* PARAMETRE :
*   une structure Plateau
*/
void affScoreCourant(Plateau p){
    printf("Score actuel : ");
    printf("%s(%c) %d", p.joueurs[0]->nom, p.joueurs[0]->symbol, p.joueurs[0]->score);
    printf(" - ");
    printf("%s(%c) %d\n", p.joueurs[1]->nom, p.joueurs[1]->symbol, p.joueurs[1]->score);
}

/**
* BUT : affiche le score final
* PARAMETRE :
*   une structure Plateau
*/
void affScoreFinal(Plateau p){
    if (p.joueurs[0]->score > p.joueurs[1]->score){
        printf("Bravo %s, vous avez gagné %d à %d.\n", p.joueurs[0]->nom, p.joueurs[0]->score, p.joueurs[1]->score);
    }
    else if (p.joueurs[0]->score < p.joueurs[1]->score){
        printf("Bravo %s, vous avez gagné %d à %d.\n", p.joueurs[1]->nom, p.joueurs[1]->score, p.joueurs[0]->score);
    }
    else{
        printf("Egalité %d à %d.\n", p.joueurs[0]->score, p.joueurs[1]->score);
    }
}




/* FIN DE PARTIE */

/**
* BUT : gere les conditions mettant fin au jeu
* PARAMETRE :
*   une structure Plateau
* RETOUR : 1 si l'un des deux joueurs ne possède plus de pion sur le plateau
*    ou si le plateau est rempli
*         0 sinon
*/
int findeJeu(Plateau p){
    int i, j;
    if (p.joueurs[0]->score == 0 || p.joueurs[1]->score == 0){
        return 1;
    }

    for (i = 0; i<TAILLE_PLATEAU+2; i++){
        for (j = 0; j<TAILLE_PLATEAU+2; j++){
            if (p.plateau[i][j] == '.'){
                return 0;
            }
        }
    }
    return 1;
}




/* CHOIX DES MODES */

/**
* BUT : gere le choix du mode d'affichage (-a pour ASCII ou -g pour Graphique)
* PARAMETRES :
*   un entier argc : le nombre de parametres passe au programme
*   un pointeur sur la chaine de caracteres argv : contient les parametres passe au programme
*   un pointeur sur un entier : mode d'affichage
*/
void choixModeAffichage(int argc, char *argv[], int *mode){
    int i;
    for (i = 1; i<argc; i++){
        if (argv[i][0] == '-'){
            if (argv[i][1] == 'a'){
                *mode = 1;
            }
            else if (argv[i][1] == 'g'){
                *mode = 2;
            }
        }
    }
}

/**
* BUT : gere le choix du mode de jeu
* (-h pour Joueur vs Joueur ou -o pour Joueur vs Ordi)
* PARAMETRES :
*   un entier argc : le nombre de parametres passe au programme
*   un pointeur sur la chaine de caracteres argv : contient les parametres passe au programme
*   un pointeur sur un entier : mode de jeu
*/
void choixModeJeu(int argc, char *argv[], int *mode){
    int i;
    for (i = 1; i<argc; i++){
        if (argv[i][0] == '-'){
            if (argv[i][1] == 'h'){
                *mode = 1;
            }
            else if (argv[i][1] == 'o'){
                *mode = 2;
            }
        }
    }
}




/* PARTIE QUI GERE L'AFFICHAGE GRAPHIQUE */

/**
* BUT : dessine la grille du plateau de jeu
*/
void dessineGrille(){
    int i;
    for(i=0; i<TAILLE_PLATEAU+1; i++){
        MLV_draw_line(MARGE, MARGE + i*TAILLE_CASE, MARGE + (TAILLE_PLATEAU) * TAILLE_CASE, MARGE + i*TAILLE_CASE, MLV_COLOR_WHITE); /*lig*/
        MLV_draw_line(MARGE + i*TAILLE_CASE, MARGE, MARGE + i*TAILLE_CASE, MARGE + (TAILLE_PLATEAU) * TAILLE_CASE, MLV_COLOR_WHITE); /*col*/
    }
    MLV_actualise_window();
}

/**
* BUT : dessine les pions
* PARAMETRE :
*   une structure Plateau
*/
void dessinePion(Plateau p){
    int xPion, yPion;
    int rayon;

    int i, j;
    for (i = 1; i<TAILLE_PLATEAU+1; i++){
        for (j = 1; j<TAILLE_PLATEAU+1; j++){
            if (p.plateau[i][j] != '.'){

                xPion =  MARGE + (j * TAILLE_CASE) - (TAILLE_CASE / 2);
                yPion =  MARGE + (i * TAILLE_CASE) - (TAILLE_CASE / 2);
                rayon = TAILLE_CASE / 2 - 1;

                if (p.plateau[i][j] == 'o'){
                    MLV_draw_filled_circle(xPion, yPion, rayon, MLV_COLOR_WHITE);
                }
                else if (p.plateau[i][j] == 'x'){
                    MLV_draw_circle(xPion, yPion, rayon, MLV_COLOR_WHITE);
                }
            }
        }
    }
    MLV_actualise_window();
}




/*POSAGE DES PIONS AVEC LA SOURIS*/

/**
* BUT :
* gere la pose d'un pion sur le plateau à l'aide de la souris
* PARAMETRES :
*   un pointeur sur la structure Plateau
*   deux structures Joueur
*/
void clic(Plateau *p, Joueur j1, Joueur j2){
    int x, y; /*coord du clic*/
    int xCase, yCase; /*coord de la case dans le plateau*/

    do {
        MLV_wait_mouse(&x, &y);

        xCase = (y-MARGE) / TAILLE_CASE + 1;
        yCase = (x-MARGE) / TAILLE_CASE + 1;
    } while (coupValide(*p, j2, xCase, yCase) != 1);

    changeColor(&(*p), j1, j2, xCase, yCase);
}




/*DESSINE LES NOMS ET SCORES*/

/**
* BUT : retourne une couleur rouge plus ou moins prononcé selon la valeur de i
* PARAMETRE :
*   un entier i : entre 0 et 255
*/
MLV_Color fabrique_couleur(int i){
    return MLV_rgba(i, 0, 0, 255);
    
}

/**
* BUT : affiche un texte decrivant les evenements courants c'est à dire le
* nom du joueur courant ainsi que son score courant
* PARAMETRES :
*   une structure Joueur
*   un entier yPos : coordonnées en y de la position du coin Nord-Ouest
* d'ou part le texte
*   un entier trans : defini la transparence de la couleur du texte à afficher
*   une chaine de caracteres couleur : la couleur du pion du joueur courant
*/
void dessineEventCourant(Joueur j, int yPos, int trans, char couleur[]){
    char score[3];
    sprintf(score, "%d", j.score);

    strcat(j.nom, " (");
    strcat(j.nom, couleur);
    strcat(j.nom, ") : ");
    strcat(j.nom, score);

    MLV_draw_text(
		MARGE, yPos, 
		j.nom,
		fabrique_couleur(trans)
	);
    MLV_actualise_window();
}

/**
* BUT : affiche un texte decrivant le score final
* PARAMETRES :
*   une structure Plateau
*   un entier yPos : coordonnées en y de la position du coin Nord-Ouest
* d'ou part le texte
*/
void dessineScoreFinal(Plateau p, int yPos){
    char scorej1[3], scorej2[3];
    char text[70] = "Score final : ";

    sprintf(scorej1, "%d", p.joueurs[0]->score);
    sprintf(scorej2, "%d", p.joueurs[1]->score);
    strcat(text, scorej1);
    strcat(text, " à ");
    strcat(text, scorej2);

    if (p.joueurs[0]->score > p.joueurs[1]->score){
        strcat(text, " -> Gagnant : ");
        strcat(text, p.joueurs[0]->nom);
    }
    else if (p.joueurs[0]->score < p.joueurs[1]->score){
        strcat(text, " -> Gagnant : ");
        strcat(text, p.joueurs[1]->nom);
    }
    else{
        strcat(text, " -> Egalité");
    }

    MLV_draw_text(
		MARGE, yPos, 
		text,
		MLV_COLOR_RED
	);
    MLV_actualise_window();
}




/*AMELIORATION*/

/**
* BUT : creer des blocs sur le plateau
* ou les joueurs ne pourront pas jouer
* PARAMETRES :
*   un pointeur sur la structure Plateau
*   un entier nbBloc : le nombre de blocs
*/
void creeBloc(Plateau *p, int nbBloc){
    int i;
    int x, y;

    for (i = 0; i<nbBloc; i++){
        do{
            x = rand() % TAILLE_PLATEAU + 1;
            y = rand() % TAILLE_PLATEAU + 1;
        } while (p->plateau[x][y] == 'b' || p->plateau[x][y] == 'o' || p->plateau[x][y] == 'x');

        p->plateau[x][y] = 'b';
    }
}

/**
* BUT : dessine les blocs sur le plateau
* PARAMETRE :
*   une structure Plateau
*/
void dessineBloc(Plateau p){
    int i, j;
    int x1, y1, x2, y2;

    for (i = 1; i<TAILLE_PLATEAU+1; i++){
        for (j = 1; j<TAILLE_PLATEAU+1; j++){
            if (p.plateau[i][j] == 'b'){

                x1 = MARGE + (j * TAILLE_CASE) - TAILLE_CASE;
                y1 = MARGE + (i * TAILLE_CASE) - TAILLE_CASE;
                x2 = MARGE + (j * TAILLE_CASE);
                y2 = MARGE + (i * TAILLE_CASE);

                MLV_draw_line(x1, y1, x2, y2, MLV_COLOR_GREEN);
                MLV_draw_line(x2, y1, x1, y2, MLV_COLOR_GREEN);

            }
        }
    }
    MLV_actualise_window();
}




/* MAIN */
int main(int argc, char *argv[]){
    srand(time(NULL));

    /*INITIALISATION*/
    int modeJeu = 0, modeAffichage = 0;
    int tour = 0;
    char blanc[6] = "blanc", noir[5] = "noir"; /*couleurs des pions*/
    int nbBloc = 5; /*nombre de bloc où l’on ne peut pas jouer*/
    Joueur j1, j2, ordi;
    Plateau p;


    /*Choix mode d'affichage*/
    choixModeAffichage(argc, &(*argv), &modeAffichage);
    if (modeAffichage == 0){
        printf("ERREUR!, vous n'avez tapé aucun mode d'affichage pour le jeu\n");
        printf("Tapez -a pour : ASCII\n");
        printf("Tapez -g pour : Graphique\n");
        exit(3);
        return 3;
    }

    /*Choix mode de jeu*/
    choixModeJeu(argc, &(*argv), &modeJeu);
    if (modeJeu == 1){ /*j vs j*/
        initJoueur(&j1, "premier", 'o');
        initJoueur(&j2, "second", 'x');
        initPlateau(&p, &j1, &j2);
    }
    else if (modeJeu == 2){ /*j vs ordi*/
        initJoueur(&j1, "premier", 'o');
        initOrdi(&ordi, 'x');
        initPlateau(&p, &j1, &ordi);
    }
    else{
        printf("ERREUR!, vous n'avez tapé aucun mode de jeu\n");
        printf("Tapez -h pour : Joueur VS Joueur\n");
        printf("Tapez -o pour : Joueur VS BOT\n");
        exit(3);
        return 3;
    }


    if (modeAffichage == 1){ /*ASCII*/
        printf("\n");
        affPlateau(p);
        printf("\n");
        while (findeJeu(p) != 1){

            if (modeJeu == 1){ /*j vs j*/
                if (tour % 2 == 0){
                    posePion(&p, j1, j2);
                }
                else{
                    posePion(&p, j2, j1);
                }
            }
            else{ /*j vs ordi*/
                if (tour % 2 == 0){
                    posePion(&p, j1, ordi);
                }
                else{
                    posePionOrdi(&p, ordi, j1);
                }
            }

            affPlateau(p);
            affScoreCourant(p);
            printf("\n");
            tour+=1;
        }
        affScoreFinal(p);
    }

    else{ /*Graphique*/
        MLV_create_window("Attaxx", "", TAILLE_PLATEAU * TAILLE_CASE + MARGE*2, TAILLE_PLATEAU * TAILLE_CASE + MARGE*2);
        creeBloc(&p, nbBloc);
    
        dessineGrille();
        dessinePion(p);
        dessineBloc(p);
        while (findeJeu(p) != 1){

            if (modeJeu == 1){ /*j vs j*/
                if (tour % 2 == 0){
                    dessineEventCourant(j1, MARGE-40, 255, blanc);
                    dessineEventCourant(j2, MARGE-20, 100, noir);
                    clic(&p, j1, j2);
                }
                else{
                    dessineEventCourant(j1, MARGE-40, 100, blanc);
                    dessineEventCourant(j2, MARGE-20, 255, noir); 
                    clic(&p, j2, j1);
                }
            }
            else{ /*j vs ordi*/
                if (tour % 2 == 0){
                    dessineEventCourant(j1, MARGE-40, 255, blanc);
                    dessineEventCourant(ordi, MARGE-20, 100, noir);
                    clic(&p, j1, ordi);
                }
                else{
                    dessineEventCourant(j1, MARGE-40, 100, blanc);
                    dessineEventCourant(ordi, MARGE-20, 255, noir); 
                    MLV_wait_seconds(1);
                    posePionOrdi(&p, ordi, j1);
                }
            }
            MLV_clear_window(MLV_COLOR_BLACK);
            dessineGrille();
            dessinePion(p);
            dessineBloc(p);
            tour+=1;
        }
        dessineScoreFinal(p, TAILLE_PLATEAU * TAILLE_CASE + MARGE + 5);

        MLV_wait_seconds(20);
        MLV_free_window();
    }
}
