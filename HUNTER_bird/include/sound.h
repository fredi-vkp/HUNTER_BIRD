/*
 * ================================================
 * HUNTER BIRD
 * Fichier  : include/sound.h
 * Role     : Declarations des fonctions son
 * Auteur   : Dev C
 * ================================================
 */

#ifndef SOUND_H
#define SOUND_H

#include "game.h"  /* Pour SDL2_mixer */

/*
 * ================================================
 * DECLARATIONS DES FONCTIONS
 * Ces fonctions sont codees dans src/sound.c
 *
 * sound_play_shoot() → joue le son du tir
 * sound_play_dead()  → joue le son oiseau mort
 * sound_play_music() → lance la musique de fond
 * sound_stop_music() → arrete la musique
 * ================================================
 */

/* Joue le son du tir */
void sound_play_shoot(Mix_Chunk *sound);

/* Joue le son quand un oiseau est abattu */
void sound_play_dead(Mix_Chunk *sound);

/* Lance la musique de fond en boucle */
void sound_play_music(Mix_Music *music);

/* Arrete la musique de fond */
void sound_stop_music(void);

#endif