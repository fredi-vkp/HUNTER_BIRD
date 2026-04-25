/*
* ================================================
* HUNTER BIRD
* Fichier : include/game.h
* Role : Structure principale + declarations
* Auteur : Dev A
* ================================================
*/
/* Protection contre la double inclusion */
/* Si game.h est inclus 2 fois, ca ne plante pas */
#ifndef GAME_H
#define GAME_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
/* ================================================ */
/* CONSTANTES */
/* ================================================ */
#define WINDOW_TITLE "Hunter Bird"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define TARGET_FPS 60
#define FRAME_DELAY (1000 / TARGET_FPS) /* ms entre chaque frame */
#define MAX_BIRDS 10
#define MAX_AMMO 3
#define HIT_RADIUS 35 /* Distance en pixels pour toucher un oiseau */
/* ================================================ */
/* ETATS DU JEU */
/* ================================================ */
/* Le jeu peut etre dans un de ces 4 etats */
/* On change d'etat selon ce qui se passe */
typedef enum {
STATE_MENU, /* Ecran d'accueil */
STATE_PLAYING, /* Partie en cours */
STATE_LEVEL_UP, /* Passage au niveau suivant */
STATE_GAME_OVER /* Ecran Game Over */
} GameState;
/* ================================================ */
/* STRUCTURE D'UN OISEAU */
/* ================================================ */
typedef struct {
float x, y; /* Position a l'ecran */
float vx, vy; /* Vitesse (pixels par frame) */
int alive; /* 1 = vivant | 0 = mort */
int escaped; /* 1 = sorti de l'ecran */
int frame; /* 0 ou 1 pour l'animation */
float anim_timer; /* Chrono pour animer */
float fall_speed; /* Vitesse de chute si mort */
} Bird;
/* ================================================ */
/* STRUCTURE DU JOUEUR */
/* ================================================ */
typedef struct {
int mouse_x, mouse_y; /* Position du viseur */
int score; /* Score total */
int lives; /* Vies restantes */
int ammo; /* Munitions restantes */
int shot_fired; /* 1 si clic ce frame */
} Player;
/* ================================================ */
/* STRUCTURE D'UN NIVEAU */
/* ================================================ */
typedef struct {
int number; /* Numero du niveau */
int bird_count; /* Nb oiseaux ce niveau */
float bird_speed; /* Vitesse des oiseaux */
int birds_shot; /* Oiseaux abattus */
int birds_escaped; /* Oiseaux echappes */
} Level;
/* ================================================ */
/* STRUCTURE PRINCIPALE DU JEU */
/* C'est ici que tout est regroupe */
/* ================================================ */
typedef struct {
/* -- Fenetre et rendu SDL2 -- */
SDL_Window *window; /* La fenetre du jeu */
SDL_Renderer *renderer; /* Moteur de dessin */
/* -- Images -- */
SDL_Texture *background; /* Fond d'ecran */
SDL_Texture *bird_frames[2]; /* Ailes haut / bas */
SDL_Texture *bird_dead; /* Oiseau mort */
SDL_Texture *crosshair; /* Viseur */
/* -- Texte -- */
TTF_Font *font; /* Police d'ecriture */
/* -- Sons -- */
Mix_Music *music; /* Musique de fond */
Mix_Chunk *sound_shoot; /* Son du tir */
Mix_Chunk *sound_dead; /* Son oiseau abattu */
/* -- Modules du jeu -- */
Bird birds[MAX_BIRDS]; /* Tableau d'oiseaux */
Player player; /* Le joueur */
Level level; /* Le niveau actuel */
GameState state; /* Etat actuel du jeu */
int running; /* 1 = jeu en cours | 0 = on quitte */
} Game;
/* ================================================ */
/* DECLARATIONS DES FONCTIONS */
/* Ces fonctions sont definies dans game.c */
/* ================================================ */
int init_game(Game *game); /* Initialise SDL2 + fenetre */
void game_loop(Game *game); /* Boucle principale du jeu */
void cleanup(Game *game); /* Libere la memoire */
#endif