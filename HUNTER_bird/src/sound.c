/*
 * ================================================
 * HUNTER BIRD
 * Fichier  : src/sound.c
 * Role     : Gestion des sons et de la musique
 * Auteur   : Dev C
 * ================================================
 *
 * CE FICHIER CONTIENT :
 *   1. sound_play_shoot() → son du tir
 *   2. sound_play_dead()  → son oiseau mort
 *   3. sound_play_music() → musique de fond
 *   4. sound_stop_music() → arreter la musique
 *
 * COMMENT SDL2_mixer FONCTIONNE :
 *   Mix_Chunk = un effet sonore court (tir, mort...)
 *   Mix_Music = une musique longue (fond sonore)
 *
 *   Mix_PlayChannel(-1, son, 0)
 *     -1  = SDL choisit le canal disponible
 *     son = le son a jouer
 *     0   = jouer une seule fois (pas de boucle)
 *
 *   Mix_PlayMusic(musique, -1)
 *     musique = la musique a jouer
 *     -1      = boucle infinie
 * ================================================
 */

#include "sound.h"  /* Declarations des fonctions */

/* ================================================
 * 1. SOUND_PLAY_SHOOT
 * Joue le son du tir quand le joueur clique
 *
 * sound → le son a jouer (charge depuis shoot.wav)
 * ================================================ */
void sound_play_shoot(Mix_Chunk *sound)
{
    /* Verifier que le son est bien charge */
    if (!sound) return;

    /* Jouer le son sur un canal disponible */
    /* -1 = SDL choisit automatiquement     */
    /* 0  = jouer une seule fois            */
    Mix_PlayChannel(-1, sound, 0);
}

/* ================================================
 * 2. SOUND_PLAY_DEAD
 * Joue le son quand un oiseau est abattu
 *
 * sound → le son a jouer (charge depuis bird_dead.wav)
 * ================================================ */
void sound_play_dead(Mix_Chunk *sound)
{
    if (!sound) return;
    Mix_PlayChannel(-1, sound, 0);
}

/* ================================================
 * 3. SOUND_PLAY_MUSIC
 * Lance la musique de fond en boucle infinie
 *
 * music → la musique a jouer (chargee depuis music.mp3)
 * ================================================ */
void sound_play_music(Mix_Music *music)
{
    if (!music) return;

    /* -1 = boucle infinie                          */
    /* La musique tourne jusqu'a Mix_HaltMusic()    */
    Mix_PlayMusic(music, -1);
}

/* ================================================
 * 4. SOUND_STOP_MUSIC
 * Arrete la musique de fond
 * Appelee au Game Over ou a la fermeture du jeu
 * ================================================ */
void sound_stop_music(void)
{
    Mix_HaltMusic();
}