#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

//TOUT CE QUI EST FAIT (Tout n'est pas cité à 100%) :
//  Les structures (Tortue, Carte, Joueur, Plateau, Jeu).
//	Les allocations mémoires (allouerTortue, allouerCarte, allouerJoueur, allouerPlateau, allouerJeu).
//	Le tableau de Tortues (leJeu->tablTortue[a]) avec "a" allant de 0 à 4.
//	Création des 5 Tortues avec chacune une couleur différente.
//	Le paquet de Cartes (leJeu->paquetCarte[b]) avec "b" allant de 0 à 51.
//	Création des 52 cartes avec les bonnes couleurs et les bonnes instructions.
//	La liste des Joueurs (leJeu->listeJoueurs[c]) avec "c" allant de 0 à 4.
//	La main des Joueurs (leJeu->listeJoueurs[c]->main[d]) avec "d" allant de 0 à 4.
//	Le mélange du tableau de Tortues.
//	Le mélange du paquet de Cartes.
//	Attribution d'une Tortue par Joueur.
//	Distribution de 5 cartes par Joueur.
//	Initialisation du plateau de Jeu et les 5 Tortues sont placées sur la première colonne du plateau.
//	Système de Jeu : Joueur par Joueur, Tour par Tour, jusqu'à ce que une tortue se retrouve sur la case [9][0] du plateau.
//	Sécurité au niveau du Plateau de Jeu :
//		Si on essaye de reculer de 1 alors que la Tortue se trouve sur la colonne de départ sa carte compte pour 0.
//		Si on essaye d'avancer de 2 alors que la Tortue se trouve sur l'avant dernière colonne sa carte comptera comme une carte +1.
//	Free de toutes les allocations faîtes.
//
//============================================
//============================================
//CE QUI PEUT ETRE AMELIORE et/ou AJOUTE :
//	Sécuriser le jeu au niveau des saisies (scanf) pour empêcher les Joueurs de rentrer n'importe quoi.
//	Créer une boucle et un booléen pour pouvoir rejouer sans relancer le programme.
//	Factoriser certaines parties de Code qui sont potentiellement encore redondantes.
// 	Création de BOTs qui nous permettent de Jouer quand on est tout seul, avec plusieurs niveaux de difficultés.
//

//============================================
//===============JEU DES TORTUES==============
//============================================
//              =================
//============================================
//=================STRUCTURES=================
//============================================
//On définit la structure "Tortue".
struct Tortue {
    int couleur;
    int rang; // Le rang n'est pas utilisé (Je fais tout avec les coordonnées [x][y] du plateau.
};

//On définit la structure "Carte".
struct Carte {
    int idCarte;
    int couleurCarte;
    int estUtilise;
    int instruction;
};

//On définit la structure "Joueur".
struct Joueur {
    char* nom;
    struct Tortue* tortue;
    struct Carte* main[5];
};

//On définit la structure "Plateau".
struct Plateau {
    struct Tortue* lescases[10][5];
};

//On définit la sturcture "Jeu".
struct Jeu {
    struct Joueur* listeJoueurs[5];
    struct Tortue* tablTortue[5];
    struct Carte* paquetCarte[52];
    struct Plateau* plateau;
    int nbJoueur;
};

//============================================
//===============ALLOCATIONS==================
//============================================
//Fonction pour allouer de la mémoire pour un struct Tortue.
struct Tortue* allouerTortue(){
    struct Tortue* allocation = malloc(sizeof(struct Tortue));
    (allocation->couleur) = 0;
    (allocation->rang) = 0;
    return allocation;
}

//Fonction pour allouer de la mémoire pour un struct Carte.
struct Carte* allouerCarte(){
    struct Carte* allocation = malloc(sizeof(struct Carte));
    (allocation->idCarte) = 0;
    (allocation->couleurCarte) = 0;
    (allocation->estUtilise) = 0;
    (allocation->instruction) = 0;
    return allocation;
};

//Fonction pour allouer de la mémoire pour un struct Joueur.
struct Joueur* allouerJoueur(){
    struct Joueur* allocation = malloc(sizeof(struct Joueur));
    (allocation->nom) = malloc(sizeof(char)*21);
    return allocation;
};

//Fonction pour allouer de la mémoire pour un struct Plateau.
struct Plateau* allouerPlateau(){
    struct Plateau* allocation = malloc(sizeof(struct Plateau));
    return allocation;
};

//Fonction pour allouer de la mémoire pour le Jeu.
struct Jeu* allouerJeu(){
    struct Jeu* allocation = malloc(sizeof(struct Jeu));
    (allocation->nbJoueur) = 0;
    for (int i = 0; i<5; i++){
        (allocation->listeJoueurs[i]) = allouerJoueur();
        (allocation->tablTortue[i]) = allouerTortue();
        (allocation->tablTortue[i]->couleur) = i+1;
    }
    for (int i = 0; i<52; i++){
        (allocation->paquetCarte[i]) = allouerCarte();
    }
    (allocation->plateau) = allouerPlateau();

    return allocation;
};

//============================================
//=================AFFICHAGE==================
//============================================
//Pour changer la couleur de l'affichage
void Color(int couleurDuTexte, int CouleurDeFond){
    HANDLE H=GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(H,CouleurDeFond*16+couleurDuTexte);
}

//Pour afficher les tortues
void afficherTortue (struct Tortue** tablTortue){
    for (int i = 0; i<5; i++){
        Color(tablTortue[i]->couleur, 0);
        printf("\nCouleur : %d \nRang : %d\n", tablTortue[i]->couleur, tablTortue[i]->rang);
    }
}

//Pour le paquet de Carte.
void afficherCarte (struct Carte** paquet){
    for (int x = 0; x<52; x++){
        printf("\nID Carte: %d, Instruction : %d, Couleur : %d", (paquet[x]->idCarte),(paquet[x]->instruction),(paquet[x]->couleurCarte));
    }
}

//Pour afficher la main des joueurs
void afficherMainJoueurs (struct Jeu* leJeu){
    for (int i = 0; i<(leJeu->nbJoueur); i++){
        printf("\nJoueur : %s", leJeu->listeJoueurs[i]->nom);
        for (int y = 0; y<5; y++){
            printf("\nMain : %d", leJeu->listeJoueurs[i]->main[y]->idCarte);
        }
    }
}

//Pour afficher "A quel joueur appartient la tortue ?"
void afficherRelationTortueJoueur(struct Jeu* leJeu){
    printf("\nJe vous laisse regarder qui a gagne la partie ;)\n");
    for (int i = 0; i<(leJeu->nbJoueur); i++){
        printf("\nTortue de couleur : ");
        Color((leJeu->listeJoueurs[i]->tortue->couleur), 0);
        printf("### ");
        Color(15, 0);
        printf("Associee au Joueur \"%s\"", (leJeu->listeJoueurs[i]->nom));
    }
}

//Pour afficher le plateau de Jeu
void afficherPlateau (struct Jeu* leJeu){
    system("cls");
    printf("Jeu par Mathis GAUTHIER\n\n");
    printf("===============================================\n");
    printf("\t\t  Jeu des Tortues\n");
    printf("===============================================\n");
    for (int y = 4; y > -1; y--){
        for (int x = 0; x < 10; x++){
            if (x==0){
                printf("\n\t\t");
            }
            if ((leJeu->plateau->lescases[x][y])=='\0'){
                printf("X ");
            } else if ((leJeu->plateau->lescases[x][y])!='\0'){
                    Color((leJeu->plateau->lescases[x][y]->couleur), 0);
                    printf("O ");
                    Color(15, 0);
                }
        }
    }
    printf("\n");
}

//Afficher les attributions des tortues (avant le lancement du jeu).
void voirAttribution(struct Jeu* leJeu){
        for (int i = 0; i < (leJeu->nbJoueur); i++){
        printf("Joueur \"%s\", Veuillez regarder la Tortue qui vous a ete attribuee,\nles autres cachez-vous les yeux ;)\n", (leJeu->listeJoueurs[i]->nom));
        system("pause");
        printf("Votre tortue est la tortue de couleur : ");
        Color((leJeu->listeJoueurs[i]->tortue->couleur), 0);
        printf("### ");
        switch (leJeu->listeJoueurs[i]->tortue->couleur){
        case 1:
            printf("Bleue\n");
            break;

        case 2:
            printf("Verte\n");
            break;

        case 3:
            printf("Turquoise\n");
            break;

        case 4:
            printf("Rouge\n");
            break;

        case 5:
            printf("Violette\n");
            break;
        }
        Color(15, 0);
        system("pause");
        system("cls");
    }
}

//Pour le README et les règles.
void readme(){
    Color(2, 0);
    printf("===============================================");
    printf("\n\t\t   BUT DU JEU :\n");
    printf("===============================================\n");
    Color(15 ,0);
    printf("1. Arriver avec sa tortue sur la derniere case\n   du plateau (en bas a droite).\n");
    printf("2. Vous pouvez donc faire ");
    Color(2, 0);
    printf("avancer ");
    Color(15, 0);
    printf("votre tortue\n   ou faire ");
    Color(4, 0);
    printf("reculer ");
    Color(15, 0);
    printf("celle des autres.\n");
    printf("3. Ainsi que utiliser les tortues des autres\n   pour faire avancer la votre.\n");
    Color(2, 0);
    printf("===============================================\n");
    Color(15, 0);
    system("pause");
    Color(4, 0);
    printf("\n===============================================");
    printf("\n\t\t   IMPORTANT !\n");
    printf("===============================================\n");
    printf("Les champs de saisies n'ont pas ete securises.\n");
    printf("Donc n'essayer pas de rentrer n'importe quoi\ndedans");
    printf(", ni de depasser les valeurs maxi indiques\nsinon je vous garanti un crash complet\ndu programme.\n");
    Color(4, 0);
    printf("===============================================\n");
    Color(15, 0);
    system("pause");
    system("cls");
    char* chaine = malloc(sizeof(char)*19);
    chaine = "BON JEU A VOUS !!!";
    int couleur = 2;
    printf("===============================================");
    printf("\n\t\t");
    for (int i = 0; (*(chaine+i)!='\0'); i++){
        Color(couleur, 0);
        printf("%c", (*(chaine+i)));
        couleur++;
        if (couleur > 5){
            couleur = 2;
        }
    }
    free(chaine);
    Color(15, 0);
    printf("\n===============================================\n\n");
    system("pause");
    system("cls");
}

//============================================
//========INITIALISATIONS / FONCTIONS=========
//============================================
void shuffleTortue(struct Tortue** montableau)
{
    struct Tortue* tortueTmp = allouerTortue();
    int n1, n2;
    for (int i = 0; i<1000; i++){
        n1 = rand() % (5);
        n2 = rand() % (5);
        tortueTmp = montableau[n1];
        montableau[n1] = montableau[n2];
        montableau[n2] = tortueTmp;
    }
}

void shuffleCarte(struct Carte** montableau)
{
    struct Carte* carteTmp = allouerCarte();
    int num1, num2;
    for (int i =0; i<1000; i++){
       num1 = rand() % (52);
       num2 = rand() % (52);
       carteTmp=montableau[num1];
       montableau[num1]=montableau[num2];
       montableau[num2]=carteTmp;
       }
}

void initialiserJeu(struct Jeu* leJeu){
    //Création des cartes.
    for (int x = 0; x<52; x++){
       (leJeu->paquetCarte[x]->idCarte) = x;
       switch (x) {
        case 0 ... 4:
            (leJeu->paquetCarte[x]->instruction)=1;
            (leJeu->paquetCarte[x]->couleurCarte)=1;
            break;

        case 5 ... 9:
            (leJeu->paquetCarte[x]->instruction)=1;
            (leJeu->paquetCarte[x]->couleurCarte)=2;
            break;

        case 10 ... 14:
            (leJeu->paquetCarte[x]->instruction)=1;
            (leJeu->paquetCarte[x]->couleurCarte)=3;
            break;

        case 15 ... 19:
            (leJeu->paquetCarte[x]->instruction)=1;
            (leJeu->paquetCarte[x]->couleurCarte)=4;
            break;

        case 20 ... 24:
            (leJeu->paquetCarte[x]->instruction)=1;
            (leJeu->paquetCarte[x]->couleurCarte)=5;
            break;

        case 25:
            (leJeu->paquetCarte[x]->instruction)=2;
            (leJeu->paquetCarte[x]->couleurCarte)=1;
            break;

        case 26:
            (leJeu->paquetCarte[x]->instruction)=2;
            (leJeu->paquetCarte[x]->couleurCarte)=2;
            break;

        case 27:
            (leJeu->paquetCarte[x]->instruction)=2;
            (leJeu->paquetCarte[x]->couleurCarte)=3;
            break;

        case 28:
            (leJeu->paquetCarte[x]->instruction)=2;
            (leJeu->paquetCarte[x]->couleurCarte)=4;
            break;

        case 29:
            (leJeu->paquetCarte[x]->instruction)=2;
            (leJeu->paquetCarte[x]->couleurCarte)=5;
            break;

        case 30 ... 31:
            (leJeu->paquetCarte[x]->instruction)=-1;
            (leJeu->paquetCarte[x]->couleurCarte)=1;
            break;

        case 32 ... 33:
            (leJeu->paquetCarte[x]->instruction)=-1;
            (leJeu->paquetCarte[x]->couleurCarte)=2;
            break;

        case 34 ... 35:
            (leJeu->paquetCarte[x]->instruction)=-1;
            (leJeu->paquetCarte[x]->couleurCarte)=3;
            break;

        case 36 ... 37:
            (leJeu->paquetCarte[x]->instruction)=-1;
            (leJeu->paquetCarte[x]->couleurCarte)=4;
            break;

        case 38 ... 39:
            (leJeu->paquetCarte[x]->instruction)=-1;
            (leJeu->paquetCarte[x]->couleurCarte)=5;
            break;

        case 40 ... 44:
            (leJeu->paquetCarte[x]->instruction)=1;
            (leJeu->paquetCarte[x]->couleurCarte)=0; //tortue de votre choix avance de 1
            break;

        case 45 ... 46:
            (leJeu->paquetCarte[x]->instruction)=-1;
            (leJeu->paquetCarte[x]->couleurCarte)=0; //tortue de votre choix recule de 1
            break;

        case 47 ... 49:
            (leJeu->paquetCarte[x]->instruction)=1;
            (leJeu->paquetCarte[x]->couleurCarte)=10; //couleur 10 veut dire au choix MAIS UNIQUEMENT dans les DERNIERES tortues
            break;

        case 50 ... 51:
            (leJeu->paquetCarte[x]->instruction)=2;
            (leJeu->paquetCarte[x]->couleurCarte)=10;
            break;
        }
    }

    //Attribution d'une Tortue à chaque Joueur.
    for (int i = 0; i<(leJeu->nbJoueur); i++) {
        (leJeu->listeJoueurs[i]->tortue) = (leJeu->tablTortue[i]);
    }

    //Initialisation du plateau de Jeu.
    for (int y = 0; y < 5; y++){
        for (int x = 0; x < 10; x++){
            (leJeu->plateau->lescases[x][y]) = '\0';
        }
    }
    int x = 0;
    for (int y = 0; y < 5; y++){
        (leJeu->plateau->lescases[x][y]) = (leJeu->tablTortue[y]);
    }
}

//Distribution des cartes aux Joueurs et retour du nombre de cartes distribués (dans le but de gérer la pioche).
int distribuerCarte(struct Jeu* monJeu){
    int y = 0;
    for (int i = 0; i<(monJeu->nbJoueur); i++){
        int x = 0;

        while (x < 5){
            (monJeu->listeJoueurs[i]->main[x]) = (monJeu->paquetCarte[y]);
            x++;
            y++;
        }
    }
    return y;
}

//Actions sur le tour de jeu en fonction de l'instruction de la Carte.
void carteInstruction(struct Carte* laCarteChoisie, struct Jeu* leJeu, int coordx, int coordy){
    int nvCoordx = 0;
    int nvCoordy = 0;
    int tortueEnPlus = 0;

    switch (laCarteChoisie->instruction){

        case 1:
            //avancer de 1
            nvCoordx = coordx + 1;
            for (int compt = 0; (coordy+compt) < 5; compt++){
                if ((leJeu->plateau->lescases[coordx][coordy+compt])!='\0'){
                    tortueEnPlus++;
                } else {

                }
            }
            for (int compt = 0; compt < 5; compt++){
                if ((leJeu->plateau->lescases[nvCoordx][compt])!='\0'){
                    nvCoordy++;
                } else {

                }
            }
            for (int i = 0; i < (tortueEnPlus); i++) {
                (leJeu->plateau->lescases[nvCoordx][nvCoordy+i])=(leJeu->plateau->lescases[coordx][coordy+i]);
                (leJeu->plateau->lescases[coordx][coordy+i]) = '\0';
            }
        break;

        case 2:
            if (coordx == 8){
                //On avance que de 1 si on est sur l'avant dernière case.
                nvCoordx = coordx + 1;
            } else {
                //On avance de 2
                nvCoordx = coordx +2;
            }


            for (int compt = 0; (coordy+compt) < 5; compt++){
                if ((leJeu->plateau->lescases[coordx][coordy+compt])!='\0'){
                    tortueEnPlus++;
                } else {

                }
            }
            for (int compt = 0; compt < 5; compt++){
                if ((leJeu->plateau->lescases[nvCoordx][compt])!='\0'){
                    nvCoordy++;
                } else {

                }
            }
            for (int i = 0; i < (tortueEnPlus+1); i++) {
                (leJeu->plateau->lescases[nvCoordx][nvCoordy+i])=(leJeu->plateau->lescases[coordx][coordy+i]);
                (leJeu->plateau->lescases[coordx][coordy+i]) = '\0';
            }

        break;

        case -1:
            nvCoordx = coordx - 1;
            if (coordx != 0){
                for (int compt = 0; (coordy+compt) < 5; compt++){
                    if ((leJeu->plateau->lescases[coordx][coordy+compt])!='\0'){
                        tortueEnPlus++;
                    } else {

                    }
                }
                for (int compt = 0; compt < 5; compt++){
                    if ((leJeu->plateau->lescases[nvCoordx][compt])!='\0'){
                        nvCoordy++;
                    } else {

                    }
                }
                for (int i = 0; i < (tortueEnPlus+1); i++) {
                    (leJeu->plateau->lescases[nvCoordx][nvCoordy+i])=(leJeu->plateau->lescases[coordx][coordy+i]);
                    (leJeu->plateau->lescases[coordx][coordy+i]) = '\0';
                }

            } else {

            }
        break;
    }
}

//LE JEU
void jeuEnCours(struct Jeu* leJeu, int carteDistrib){
    //Le JEU Action ! système Joueur par Joueur
    int pourBoucle = 1;
    while (pourBoucle==1){
        if ((leJeu->plateau->lescases[9][0])=='\0'){
            for (int i = 0; (i < (leJeu->nbJoueur)) && ((leJeu->plateau->lescases[9][0])=='\0'); i++){
                int a = 0;
                int selection = 0;
                struct Carte* carteChoisie = allouerCarte();
                afficherPlateau(leJeu);
                printf("\nJoueur \"%s\" A vous de jouer !\n", leJeu->listeJoueurs[i]->nom);
                printf("Vos cartes :\n");
                for (int y = 0; y < 5; y++){
                    a = y+1;
                    if ((leJeu->listeJoueurs[i]->main[y]->couleurCarte)==0){
                        Color(15, 0);
                    } else if ((leJeu->listeJoueurs[i]->main[y]->couleurCarte)==10){
                        Color(15, 0);
                        printf("(Special)");
                    } else {
                        Color((leJeu->listeJoueurs[i]->main[y]->couleurCarte), 0);
                    }
                    printf("%d. Instruction : %d | ID : %d\n", a,(leJeu->listeJoueurs[i]->main[y]->instruction), (leJeu->listeJoueurs[i]->main[y]->idCarte));
                    Color(15, 0);
                }
                printf("Veuillez choisir une carte : ");
                scanf("%d", &selection);

                carteChoisie = (leJeu->listeJoueurs[i]->main[selection-1]);

                int coordx = 0;
                int coordy = 0;
                int boucle = 1;

                //Si couleur = 1 ou 2 ou 3 ou 4 ou 5 :
                if (((carteChoisie->couleurCarte)==1)||(carteChoisie->couleurCarte==2)||(carteChoisie->couleurCarte==3)||(carteChoisie->couleurCarte==4)||(carteChoisie->couleurCarte==5)){
                    while (boucle==1){
                        while (((leJeu->plateau->lescases[coordx][coordy])=='\0')){
                            if (coordx < 9){
                                coordx++;
                            }
                            else {
                                coordx = 0;
                                coordy++;
                            }
                        }
                        if ((carteChoisie->couleurCarte)==(leJeu->plateau->lescases[coordx][coordy]->couleur)){
                            carteInstruction(carteChoisie, leJeu, coordx, coordy);
                            (leJeu->listeJoueurs[i]->main[selection-1]) = '\0';
                            if (carteDistrib < 52){
                                (leJeu->listeJoueurs[i]->main[selection-1]) = (leJeu->paquetCarte[carteDistrib]);
                                carteDistrib++;
                            } else {

                            }
                            boucle = 0;
                        } else {
                            if (coordx < 9){
                                coordx++;
                            }
                            else {
                                coordx = 0;
                                coordy++;
                            }
                        }
                    }

                //Si couleur = 0 : alors il faut choisir la Tortue que l'on veut.
                } else if ((carteChoisie->couleurCarte)==0){
                    int ii = 0;
                    int selectionTortue = 0;
                    struct Tortue* tortueChoisie = allouerTortue();
                    printf("\n");
                    for (int i = 0; i < 5; i++){
                        ii = i+1;
                        printf("%d. ", ii);
                        Color((leJeu->tablTortue[i]->couleur), 0);
                        printf("Tortue %d\n", ii);
                        Color(15 ,0);
                    }
                    printf("\nVeuillez choisir la tortue sur laquelle appliquer la carte : ");
                    scanf("%d", &selectionTortue);
                    tortueChoisie = (leJeu->tablTortue[selectionTortue-1]);

                    while (boucle==1){
                        while (((leJeu->plateau->lescases[coordx][coordy])=='\0')){
                            if (coordx < 9){
                                coordx++;
                            }
                            else {
                                coordx = 0;
                                coordy++;
                            }
                        }
                        if ((tortueChoisie->couleur)==(leJeu->plateau->lescases[coordx][coordy]->couleur)){
                            carteInstruction(carteChoisie, leJeu, coordx, coordy);
                            (leJeu->listeJoueurs[i]->main[selection-1]) = '\0';
                            if (carteDistrib < 52){
                                (leJeu->listeJoueurs[i]->main[selection-1]) = (leJeu->paquetCarte[carteDistrib]);
                                carteDistrib++;
                            } else {

                            }
                            boucle = 0;
                        } else {
                            if (coordx < 9){
                                coordx++;
                            }
                            else {
                                coordx = 0;
                                coordy++;
                            }
                        }
                    }

                //Si la couleur  = 10 : alors on choisit l'une des tortues situées en dernière position.
                } else if ((carteChoisie->couleurCarte)==10){
                    int iii = 0;
                    int nbTortues = 0;
                    int selectionneLaTortue = 0;
                    struct Tortue* laTortueChoisie = allouerTortue();
                    while ((leJeu->plateau->lescases[coordx][coordy])=='\0'){
                        if (coordx < 9){
                            coordx++;
                        }
                        else {

                        }
                    }
                    if ((leJeu->plateau->lescases[coordx][coordy])!='\0'){
                        for (int i = 0; (coordy+i) < 5; i++){
                            if ((leJeu->plateau->lescases[coordx][coordy+i])!='\0'){
                                nbTortues++;
                            }
                            else {

                            }
                        }
                        printf("\n");

                        for (int i = 0; i < nbTortues; i++){
                            iii = i+1;
                            printf("%d. ", iii);
                            Color((leJeu->plateau->lescases[coordx][coordy+i]->couleur), 0);
                            printf("Tortue %d\n", iii);
                            Color(15, 0);
                        }
                        printf("\nVeuillez choisir une tortue sur laquelle appliquer la carte : ");
                        scanf("%d", &selectionneLaTortue);
                        laTortueChoisie = (leJeu->plateau->lescases[coordx][selectionneLaTortue-1]);
                        coordx = 0;
                        coordy = 0;

                        while (boucle==1){
                            while (((leJeu->plateau->lescases[coordx][coordy])=='\0')){
                                if (coordx < 9){
                                    coordx++;
                                }
                                else {
                                    coordx = 0;
                                    coordy++;
                                }
                            }
                            if ((laTortueChoisie->couleur)==(leJeu->plateau->lescases[coordx][coordy]->couleur)){
                                carteInstruction(carteChoisie, leJeu, coordx, coordy);
                                (leJeu->listeJoueurs[i]->main[selection-1]) = '\0';
                                if (carteDistrib < 52){
                                    (leJeu->listeJoueurs[i]->main[selection-1]) = (leJeu->paquetCarte[carteDistrib]);
                                    carteDistrib++;
                                } else {

                                }
                                boucle = 0;
                            } else {
                                if (coordx < 9){
                                    coordx++;
                                }
                                else {
                                    coordx = 0;
                                    coordy++;
                                }
                            }
                        }
                    }
                }
                free(carteChoisie);
            }
        }

        else {
            system("cls");
            afficherPlateau(leJeu);
            switch (leJeu->plateau->lescases[9][0]->couleur){

            case 1:
                Color((leJeu->plateau->lescases[9][0]->couleur), 0);
                printf("\nLa tortue Bleue a gagne ! Felicitations !\n");
                Color(15, 0);
                afficherRelationTortueJoueur(leJeu);
                printf("\n");
                pourBoucle = 0;
                break;

            case 2:
                Color((leJeu->plateau->lescases[9][0]->couleur), 0);
                printf("\nLa tortue Verte a gagne ! Felicitations !\n");
                Color(15, 0);
                afficherRelationTortueJoueur(leJeu);
                printf("\n");
                pourBoucle = 0;
                break;

            case 3:
                Color((leJeu->plateau->lescases[9][0]->couleur), 0);
                printf("\nLa tortue Turquoise a gagne ! Felicitations !\n");
                Color(15, 0);
                afficherRelationTortueJoueur(leJeu);
                printf("\n");
                pourBoucle = 0;
                break;

            case 4:
                Color((leJeu->plateau->lescases[9][0]->couleur), 0);
                printf("\nLa tortue Rouge a gagne ! Felicitations !\n");
                Color(15, 0);
                afficherRelationTortueJoueur(leJeu);
                printf("\n");
                pourBoucle = 0;
                break;

            case 5:
                Color((leJeu->plateau->lescases[9][0]->couleur), 0);
                printf("\nLa tortue Violette a gagne ! Felicitations !\n");
                Color(15, 0);
                afficherRelationTortueJoueur(leJeu);
                printf("\n");
                pourBoucle = 0;
                break;
            }
        }
    }
}

//Fonction pour Free toutes les allocations.
void libererMemoire(struct Jeu* leJeu){
    for (int i = 0; i<5; i++){
        for (int y = 0; y<5; y++){
            free(leJeu->listeJoueurs[i]->main[y]);
        }
        free(leJeu->listeJoueurs[i]->nom);
        free(leJeu->listeJoueurs[i]);
    }
    for (int i = 0; i<52; i++){
        free(leJeu->paquetCarte[i]);
    }
    free(leJeu->plateau);
    for (int i = 0; i<5; i++){
        free(leJeu->tablTortue[i]);
    }
    free(leJeu);
}

//Fonction (choix nombre de joueur, et leur nom...).
void playerRegister(struct Jeu* leJeu){
    int var = 0;
        printf("Entrez le nombre de Joueur (1 a 5 Max) : ");
        scanf("%d", &(leJeu->nbJoueur));
        printf("\nIl y a %d joueurs", leJeu->nbJoueur);
        for (int i = 0; i<(leJeu->nbJoueur); i++){
            var = i+1;
            printf("\nEntrer votre nom Joueur %d: ", var);
            scanf("%s", (leJeu->listeJoueurs[i]->nom));
        }
    system("cls");
}
//============================================
//===================MAIN=====================
//============================================
int main (void)
{
    srand(time(NULL));
    int carteDistrib = 0;
    struct Jeu* leJeu = allouerJeu();
    //README + Règles
    readme();
    //Nombre de Joueur + Nom des Joueurs.
    playerRegister(leJeu);
    //Mélange des tortues
    shuffleTortue(leJeu->tablTortue);
    //Création des cartes et Attribution des Tortues aux Joueurs et Initialisation du plateau de Jeu.
    initialiserJeu(leJeu);
    //Mélange des cartes
    shuffleCarte(leJeu->paquetCarte);
    //Distribuer les cartes et Récupérer le nombre de cartes qui ont été distribuées.
    carteDistrib = distribuerCarte(leJeu);
    //Voir les tortues qui ont été attribuées à chaque joueur.
    voirAttribution(leJeu);
    //Lancement du Jeu
    jeuEnCours(leJeu, carteDistrib);
    //LES FREEs de toutes les allocations.
    libererMemoire(leJeu);
    return 0;
}
