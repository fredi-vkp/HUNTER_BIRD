/*
 * ================================================
 * HUNTER BIRD
 * Fichier  : include/menu.h
 * Role     : Declarations des fonctions menus
 * Auteur   : Dev A
 * ================================================
 */

#ifndef MENU_H
#define MENU_H

#include "game.h"  /* Pour SDL2, TTF_Font */

/*
 * ================================================
 * DECLARATIONS DES FONCTIONS
 * Ces fonctions sont codees dans src/menu.c
 *
 * menu_draw_start()   → ecran d'accueil
 * menu_draw_levelup() → ecran passage de niveau
 * menu_draw_gameover()→ ecran game over
 * ================================================
 */

/* Dessine l'ecran d'accueil */
void menu_draw_start(SDL_Renderer *renderer, TTF_Font *font);

/* Dessine l'ecran de passage au niveau suivant */
void menu_draw_levelup(SDL_Renderer *renderer, TTF_Font *font,
                       int level, int score);

/* Dessine l'ecran Game Over avec le score final */
void menu_draw_gameover(SDL_Renderer *renderer, TTF_Font *font,
                        int score);

#endif