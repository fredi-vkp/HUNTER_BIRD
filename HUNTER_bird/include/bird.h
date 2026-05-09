/*
 * ================================================
 * HUNTER BIRD
 * Fichier  : include/bird.h
 * Role     : Declarations des fonctions oiseaux
 * Auteur   : Dev B
 * ================================================
 */

#ifndef BIRD_H
#define BIRD_H

#include "game.h"  /* Pour avoir acces a Bird, WINDOW_WIDTH, etc. */

/*
 * ================================================
 * DECLARATIONS DES FONCTIONS
 * Ces fonctions sont codees dans src/bird.c
 *
 * bird_spawn()      → cree un oiseau
 * bird_update()     → fait bouger un oiseau
 * bird_draw()       → dessine un oiseau
 * birds_spawn_all() → cree tous les oiseaux
 * birds_update_all()→ met a jour tous les oiseaux
 * birds_draw_all()  → dessine tous les oiseaux
 * ================================================
 */

/* Initialise un seul oiseau avec une vitesse donnee */
void bird_spawn(Bird *b, float speed);

/* Met a jour la position et l'animation d'un oiseau */
void bird_update(Bird *b);

/* Dessine un seul oiseau a l'ecran */
void bird_draw(Bird *b, SDL_Renderer *renderer,
               SDL_Texture *frames[2], SDL_Texture *dead);

/* Initialise TOUS les oiseaux du niveau */
void birds_spawn_all(Bird birds[], int count, float speed);

/* Met a jour TOUS les oiseaux */
void birds_update_all(Bird birds[], int count);

/* Dessine TOUS les oiseaux */
void birds_draw_all(Bird birds[], int count,
                    SDL_Renderer *renderer,
                    SDL_Texture *frames[2], SDL_Texture *dead);

#endif