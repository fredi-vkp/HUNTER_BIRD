/*
 * ================================================
 * HUNTER BIRD
 * Fichier  : include/player.h
 * Role     : Declarations des fonctions joueur
 * Auteur   : Dev C
 * ================================================
 */

#ifndef PLAYER_H
#define PLAYER_H

#include "game.h"  /* Pour avoir acces a Player, SDL2, etc. */

/*
 * ================================================
 * DECLARATIONS DES FONCTIONS
 * Ces fonctions sont codees dans src/player.c
 *
 * player_init()          → initialise le joueur
 * player_draw_crosshair()→ dessine le viseur
 * player_shoot()         → gere le tir
 * ================================================
 */

/* Initialise le joueur : score=0, vies=3, ammo=3 */
void player_init(Player *p);

/* Dessine le viseur a la position de la souris */
void player_draw_crosshair(Player *p, SDL_Renderer *renderer,
                            SDL_Texture *crosshair);

/* Verifie si un tir touche un oiseau */
/* Retourne 1 si touche, 0 sinon      */
int player_shoot(Player *p, Bird birds[], int count, int level);

#endif