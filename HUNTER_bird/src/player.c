/*
 * ================================================
 * HUNTER BIRD
 * Fichier  : src/player.c
 * Role     : Viseur, tir, munitions, score
 * Auteur   : Dev C
 * ================================================
 *
 * CE FICHIER CONTIENT :
 *   1. player_init()           → initialiser le joueur
 *   2. player_draw_crosshair() → dessiner le viseur
 *   3. player_shoot()          → gerer le tir
 * ================================================
 */

#include "player.h"  /* Declarations des fonctions  */
#include <math.h>    /* Pour sqrtf()                */

/* ================================================
 * 1. PLAYER_INIT
 * Initialise le joueur au debut du jeu
 * Remet tout a zero
 * ================================================ */
void player_init(Player *p)
{
    p->score      = 0;        /* Score de depart     */
    p->lives      = 3;        /* 3 vies au depart    */
    p->ammo       = MAX_AMMO; /* Munitions pleines   */
    p->mouse_x    = 0;        /* Viseur au coin      */
    p->mouse_y    = 0;
    p->shot_fired = 0;        /* Pas de tir au debut */
}

/* ================================================
 * 2. PLAYER_DRAW_CROSSHAIR
 * Dessine le viseur a la position de la souris
 *
 * p         → le joueur (contient mouse_x, mouse_y)
 * renderer  → le pinceau SDL2
 * crosshair → texture du viseur (peut etre NULL)
 * ================================================ */
void player_draw_crosshair(Player *p, SDL_Renderer *renderer,
                            SDL_Texture *crosshair)
{
    int x = p->mouse_x; /* Position X du viseur */
    int y = p->mouse_y; /* Position Y du viseur */

    if (crosshair) {
        /* On a une texture → on la dessine */
        SDL_Rect dst = { x - 20, y - 20, 40, 40 };
        SDL_RenderCopy(renderer, crosshair, NULL, &dst);

    } else {
        /* Pas de texture → on dessine un viseur en SDL2 */
        /* Croix rouge + cercle                           */

        int r = 18; /* Rayon du cercle */

        SDL_SetRenderDrawColor(renderer, 255, 50, 50, 255);

        /* Ligne horizontale */
        SDL_RenderDrawLine(renderer, x - r, y, x + r, y);

        /* Ligne verticale */
        SDL_RenderDrawLine(renderer, x, y - r, x, y + r);

        /* Cercle : on dessine 36 petits segments          */
        /* C'est la facon la plus simple de faire un cercle */
        /* en SDL2 qui n'a pas de fonction cercle native   */
        for (int i = 0; i < 36; i++) {
            /* Angle du segment en radians */
            float a1 = (float)i       * 3.14159f * 2.0f / 36.0f;
            float a2 = (float)(i + 1) * 3.14159f * 2.0f / 36.0f;

            /* Coordonnees des deux extremites du segment */
            int x1 = x + (int)(r * cosf(a1));
            int y1 = y + (int)(r * sinf(a1));
            int x2 = x + (int)(r * cosf(a2));
            int y2 = y + (int)(r * sinf(a2));

            SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
        }
    }
}

/* ================================================
 * 3. PLAYER_SHOOT
 * Verifie si le tir du joueur touche un oiseau
 *
 * p      → le joueur (position du clic, ammo, score)
 * birds  → tableau de tous les oiseaux
 * count  → nombre d'oiseaux
 * level  → numero du niveau (pour les points bonus)
 *
 * Retourne 1 si un oiseau est touche, 0 sinon
 * ================================================ */
int player_shoot(Player *p, Bird birds[], int count, int level)
{
    /* Pas de tir si pas de munitions */
    if (p->ammo <= 0) return 0;

    /* Consommer une munition */
    p->ammo--;

    /* Verifier chaque oiseau */
    for (int i = 0; i < count && i < MAX_BIRDS; i++) {

        /* Ignorer les oiseaux deja morts */
        if (!birds[i].alive) continue;

        /* Calculer la distance entre le clic et l'oiseau */
        /* Formule : distance = sqrt( dx² + dy² )         */
        float dx   = (float)p->mouse_x - birds[i].x;
        float dy   = (float)p->mouse_y - birds[i].y;
        float dist = sqrtf(dx * dx + dy * dy);

        /* Si la distance est inferieure au rayon de collision */
        if (dist < HIT_RADIUS) {

            /* L'oiseau est touche ! */
            birds[i].alive      = 0;    /* Il meurt        */
            birds[i].fall_speed = 0.0f; /* Commence a tomber */

            /* Points = 10 x numero du niveau              */
            /* Plus le niveau est eleve, plus on gagne     */
            p->score += 10 * level;

            return 1; /* Touche ! */
        }
    }

    /* Recharger automatiquement si plus de munitions */
    if (p->ammo <= 0)
        p->ammo = MAX_AMMO;

    return 0; /* Pas touche */
}