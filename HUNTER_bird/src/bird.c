/*
 * ================================================
 * HUNTER BIRD
 * Fichier  : src/bird.c
 * Role     : Spawn, mouvement, animation des oiseaux
 * Auteur   : Dev B
 * ================================================
 *
 * CE FICHIER CONTIENT :
 *   1. bird_spawn()       → creer un oiseau
 *   2. bird_update()      → faire bouger un oiseau
 *   3. bird_draw()        → dessiner un oiseau
 *   4. birds_spawn_all()  → creer tous les oiseaux
 *   5. birds_update_all() → mettre a jour tous
 *   6. birds_draw_all()   → dessiner tous
 * ================================================
 */

#include "bird.h"   /* Declarations des fonctions   */
#include <stdlib.h> /* Pour rand()                  */
#include <math.h>   /* Pour sinf()                  */

/* Taille d'un oiseau a l'ecran en pixels */
#define BIRD_WIDTH  50
#define BIRD_HEIGHT 40

/* ================================================
 * 1. BIRD_SPAWN
 * Initialise un oiseau avec une position aleatoire
 * et une vitesse donnee
 *
 * b     → pointeur vers l'oiseau a initialiser
 * speed → vitesse de deplacement
 * ================================================ */
void bird_spawn(Bird *b, float speed)
{
    /* Choisir aleatoirement le cote de depart      */
    /* rand() % 2 donne 0 ou 1                      */
    int side = rand() % 2;

    if (side == 0) {
        /* Cote gauche → va vers la droite */
        b->x  = -30.0f;   /* Commence hors ecran a gauche */
        b->vx = speed;    /* Va vers la droite            */
    } else {
        /* Cote droit → va vers la gauche */
        b->x  = WINDOW_WIDTH + 30.0f; /* Hors ecran a droite */
        b->vx = -speed;               /* Va vers la gauche   */
    }

    /* Position verticale aleatoire dans le haut de l'ecran */
    /* rand() % 250 donne un nombre entre 0 et 249          */
    b->y = 50.0f + (float)(rand() % 250);

    /* Vitesse verticale nulle au depart */
    b->vy = 0.0f;

    /* L'oiseau est vivant */
    b->alive   = 1;
    b->escaped = 0;

    /* Animation : commence a la frame 0 */
    b->frame      = 0;
    b->anim_timer = 0.0f;

    /* Pas de chute au depart */
    b->fall_speed = 0.0f;
}

/* ================================================
 * 2. BIRD_UPDATE
 * Met a jour la position et l'etat d'un oiseau
 * Appelee 60 fois par seconde
 *
 * b → pointeur vers l'oiseau a mettre a jour
 * ================================================ */
void bird_update(Bird *b)
{
    /* Si l'oiseau est mort → il tombe */
    if (!b->alive) {
        b->fall_speed += 0.5f; /* Gravite : accelere vers le bas */
        b->y += b->fall_speed;
        return; /* On s'arrete la, pas besoin du reste */
    }

    /* ── Deplacement horizontal ── */
    b->x += b->vx;

    /* ── Vol sinusoidal ──────────────────────────
     * sinf() donne une valeur qui oscille entre -1 et 1
     * Ca fait onduler l'oiseau de haut en bas
     * comme un vrai oiseau qui vole
     * ─────────────────────────────────────────── */
    b->y += sinf(b->x * 0.04f) * 1.5f;

    /* ── Rebond sur les bords haut et bas ── */
    if (b->y < 20.0f)
        b->vy = 1.0f;   /* Trop haut → redescend  */
    if (b->y > WINDOW_HEIGHT - 80)
        b->vy = -1.0f;  /* Trop bas  → remonte     */
    b->y += b->vy;

    /* ── Animation des ailes ─────────────────────
     * On change de frame toutes les 15 frames
     * frame 0 = ailes en haut
     * frame 1 = ailes en bas
     * ─────────────────────────────────────────── */
    b->anim_timer += 1.0f;
    if (b->anim_timer >= 15.0f) {
        b->frame      = 1 - b->frame; /* Alterne 0 et 1 */
        b->anim_timer = 0.0f;
    }

    /* ── Detecter si l'oiseau s'echappe ── */
    if (b->x < -60.0f || b->x > WINDOW_WIDTH + 60.0f) {
        b->escaped = 1; /* Sorti de l'ecran ! */
    }
}

/* ================================================
 * 3. BIRD_DRAW
 * Dessine un seul oiseau a l'ecran
 *
 * b        → l'oiseau a dessiner
 * renderer → le pinceau SDL2
 * frames   → textures [0]=ailes haut [1]=ailes bas
 * dead     → texture oiseau mort
 * ================================================ */
void bird_draw(Bird *b, SDL_Renderer *renderer,
               SDL_Texture *frames[2], SDL_Texture *dead)
{
    /* Rectangle ou sera dessine l'oiseau */
    SDL_Rect dst = {
        (int)b->x - BIRD_WIDTH / 2, /* x centre sur la position */
        (int)b->y - BIRD_HEIGHT / 2, /* y centre sur la position */
        BIRD_WIDTH,                  /* largeur                  */
        BIRD_HEIGHT
                           /* hauteur                  */
    };

    /* ── Oiseau mort ── */
    if (!b->alive) {
        if (dead) {
            /* Dessine la texture de mort */
            SDL_RenderCopy(renderer, dead, NULL, &dst);
        } else {
            /* Pas de texture → rectangle rouge */
            SDL_SetRenderDrawColor(renderer, 200, 50, 50, 200);
            SDL_RenderFillRect(renderer, &dst);
        }
        return;
    }

    /* ── Oiseau vivant ── */
    SDL_Texture *tex = frames[b->frame]; /* frame 0 ou 1 */

    if (tex) {
        /* On a une texture → on la dessine             */
        /* Si l'oiseau va a gauche on retourne le sprite */
        SDL_RendererFlip flip = (b->vx < 0)
            ? SDL_FLIP_HORIZONTAL  /* Retourne */
            : SDL_FLIP_NONE;       /* Normal   */

        SDL_RenderCopyEx(renderer, tex, NULL, &dst,
                         0, NULL, flip);
    } else {
        /* Pas de texture → on dessine des formes SDL2 */

        /* Corps de l'oiseau */
        SDL_SetRenderDrawColor(renderer, 80, 60, 200, 255);
        SDL_RenderFillRect(renderer, &dst);

        /* Aile animee : monte si frame 0, descend si frame 1 */
        SDL_Rect aile = {
            dst.x + 10,
            b->frame == 0 ? dst.y - 8 : dst.y + 20,
            30, 8
        };
        SDL_SetRenderDrawColor(renderer, 50, 40, 170, 255);
        SDL_RenderFillRect(renderer, &aile);

        /* Tete */
        SDL_Rect tete = { dst.x + 32, dst.y + 5, 16, 16 };
        SDL_SetRenderDrawColor(renderer, 100, 80, 220, 255);
        SDL_RenderFillRect(renderer, &tete);

        /* Bec */
        SDL_Rect bec = { dst.x + 46, dst.y + 10, 8, 5 };
        SDL_SetRenderDrawColor(renderer, 255, 180, 0, 255);
        SDL_RenderFillRect(renderer, &bec);
    }
}

/* ================================================
 * 4. BIRDS_SPAWN_ALL
 * Initialise tous les oiseaux d'un niveau
 *
 * birds → tableau de tous les oiseaux
 * count → nombre d'oiseaux a creer
 * speed → vitesse de depart
 * ================================================ */
void birds_spawn_all(Bird birds[], int count, float speed)
{
    for (int i = 0; i < count && i < MAX_BIRDS; i++) {
        bird_spawn(&birds[i], speed);
    }
}

/* ================================================
 * 5. BIRDS_UPDATE_ALL
 * Met a jour tous les oiseaux
 * ================================================ */
void birds_update_all(Bird birds[], int count)
{
    for (int i = 0; i < count && i < MAX_BIRDS; i++) {
        bird_update(&birds[i]);
    }
}

/* ================================================
 * 6. BIRDS_DRAW_ALL
 * Dessine tous les oiseaux
 * ================================================ */
void birds_draw_all(Bird birds[], int count,
                    SDL_Renderer *renderer,
                    SDL_Texture *frames[2], SDL_Texture *dead)
{
    for (int i = 0; i < count && i < MAX_BIRDS; i++) {
        bird_draw(&birds[i], renderer, frames, dead);
    }
}