/*
 * ================================================
 * HUNTER BIRD
 * Fichier  : src/hud.c
 * Role     : Affichage score, vies, munitions
 * Auteur   : Dev D
 * ================================================
 *
 * CE FICHIER CONTIENT :
 *   1. draw_text()  → fonction utilitaire pour ecrire du texte
 *   2. hud_draw()   → dessine tout le HUD
 * ================================================
 */

#include "hud.h"   /* Declarations des fonctions */
#include <stdio.h> /* Pour snprintf()             */

/* ================================================
 * 1. DRAW_TEXT (fonction interne)
 * Affiche du texte a l'ecran a une position donnee
 * static = utilisable uniquement dans ce fichier
 *
 * renderer → le pinceau SDL2
 * font     → la police de texte
 * text     → le texte a afficher
 * x, y     → position sur l'ecran
 * r, g, b  → couleur du texte (rouge, vert, bleu)
 * ================================================ */
static void draw_text(SDL_Renderer *renderer, TTF_Font *font,
                      const char *text, int x, int y,
                      Uint8 r, Uint8 g, Uint8 b)
{
    /* Si pas de police chargee on ne peut pas ecrire */
    if (!font) return;

    /* Definir la couleur du texte */
    SDL_Color color = { r, g, b, 255 };

    /* Creer une surface avec le texte rendu */
    /* TTF_RenderText_Solid = rendu simple et rapide */
    SDL_Surface *surface = TTF_RenderText_Solid(font, text, color);
    if (!surface) return;

    /* Convertir la surface en texture pour l'afficher */
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    /* Rectangle ou sera affiche le texte */
    /* surface->w et surface->h = taille exacte du texte */
    SDL_Rect dst = { x, y, surface->w, surface->h };

    /* Afficher le texte */
    SDL_RenderCopy(renderer, texture, NULL, &dst);

    /* Liberer la memoire */
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

/* ================================================
 * 2. HUD_DRAW
 * Dessine tout le HUD en haut de l'ecran
 *
 * ┌──────────────────────────────────────────┐
 * │ ●●●  SCORE: 120  NIV: 2  ❤ ❤ ❤         │
 * └──────────────────────────────────────────┘
 *
 * renderer → le pinceau SDL2
 * font     → police pour le texte
 * p        → le joueur (score, vies, ammo)
 * l        → le niveau actuel
 * ================================================ */
void hud_draw(SDL_Renderer *renderer, TTF_Font *font,
              Player *p, Level *l)
{
    char buf[64]; /* Buffer pour construire les textes */

    /* ── Barre noire semi-transparente en haut ── */
    /* SDL_BLENDMODE_BLEND = permet la transparence */
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150);
    SDL_Rect barre = { 0, 0, WINDOW_WIDTH, 45 };
    SDL_RenderFillRect(renderer, &barre);

    /* ── Munitions : cercles jaunes ou gris ── */
    /* Jaune = munition disponible              */
    /* Gris  = munition utilisee               */
    for (int i = 0; i < MAX_AMMO; i++) {
        if (i < p->ammo)
            /* Jaune = munition restante */
            SDL_SetRenderDrawColor(renderer, 255, 200, 0, 255);
        else
            /* Gris = munition vide */
            SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);

        SDL_Rect balle = { 15 + i * 28, 13, 20, 20 };
        SDL_RenderFillRect(renderer, &balle);
    }

    /* ── Score ── */
    /* snprintf construit la chaine "SCORE: 120" */
    snprintf(buf, sizeof(buf), "SCORE: %d", p->score);
    draw_text(renderer, font, buf, 120, 10, 255, 220, 50);

    /* ── Niveau ── */
    snprintf(buf, sizeof(buf), "NIVEAU: %d", l->number);
    draw_text(renderer, font, buf, 340, 10, 200, 200, 255);

    /* ── Vies : carres rouges ── */
    for (int i = 0; i < p->lives; i++) {
        SDL_SetRenderDrawColor(renderer, 220, 50, 50, 255);
        SDL_Rect vie = { 620 + i * 35, 12, 25, 22 };
        SDL_RenderFillRect(renderer, &vie);
    }

    /* ── Oiseaux restants ── */
    int restants = l->bird_count - l->birds_shot;
    snprintf(buf, sizeof(buf), "OISEAUX: %d", restants);
    draw_text(renderer, font, buf, 730, 10, 150, 255, 150);
}