/*
========================================
HUNTER BIRD
Fichier  : src/main.c
Role     : Point d'entree du programme
Auteur   : Dev A
========================================
*/

#include <stdio.h>
#include <stdlib.h>
#include "game.h"

int main(int argc, char *argv[])
{
    Game game = {0};

    /* Etape 1 : Initialiser SDL2 et creer la fenetre */
    if (!init_game(&game)) {
        printf("Erreur : impossible de lancer le jeu\n");
        cleanup(&game);
        return EXIT_FAILURE;
    }

    /* Etape 2 : Lancer la boucle de jeu */
    printf("Jeu lance ! Appuie sur Echap pour quitter\n");
    game_loop(&game);

    /* Etape 3 : Nettoyer la memoire */
    cleanup(&game);

    return EXIT_SUCCESS;
}