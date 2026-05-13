/*
 * ================================================
 * HUNTER BIRD
 * Fichier  : src/menu.c
 * Role     : Ecrans start, niveau suivant, game over
 * Auteur   : Dev A
 * ================================================
 *
 * CE FICHIER CONTIENT :
 *   1. draw_text_centered() → texte centre a l'ecran
 *   2. menu_draw_start()    → ecran d'accueil
 *   3. menu_draw_levelup()  → ecran niveau suivant
 *   4. menu_draw_gameover() → ecran game over
 * ================================================
 */

#include "menu.h"  /* Declarations des fonctions */
#include <stdio.h> /* Pour snprintf()             */

/* ================================================
 * 1. DRAW_TEXT_CENTERED (fonction interne)
 * Affiche du texte centre horizontalement
 *
 * renderer → le pinceau SDL2
 * font     → la police de texte
 * text     → le texte a afficher
 * y        → position verticale
 * r, g, b  → couleur du texte
 * ================================================ */
static void draw_text_centered(SDL_Renderer *renderer, TTF_Font *font,
                                const char *text, int y,
                                Uint8 r, Uint8 g, Uint8 b)
{
    if (!font) return;

    SDL_Color color = { r, g, b, 255 };

    /* Creer la surface avec le texte */
    SDL_Surface *surface = TTF_RenderText_Solid(font, text, color);
    if (!surface) return;

    /* Convertir en texture */
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    /* Centrer horizontalement :                        */
    /* x = (largeur ecran - largeur texte) / 2         */
    SDL_Rect dst = {
        (WINDOW_WIDTH - surface->w) / 2,  /* Centre en X */
        y,                                 /* Position Y  */
        surface->w,
        surface->h
    };

    SDL_RenderCopy(renderer, texture, NULL, &dst);

    /* Liberer la memoire */
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

/* ================================================
 * 2. MENU_DRAW_START
 * Dessine l'ecran d'accueil
 *
 * ┌──────────────────────────────┐
 * │                              │
 * │        HUNTER BIRD           │  ← titre
 * │                              │
 * │   Appuie sur ENTREE          │  ← instruction
 * │   pour commencer             │
 * │                              │
 * └──────────────────────────────┘
 * ================================================ */
void menu_draw_start(SDL_Renderer *renderer, TTF_Font *font)
{
    /* Fond bleu ciel degrade */
    SDL_SetRenderDrawColor(renderer, 30, 100, 200, 255);
    SDL_RenderClear(renderer);

    /* Sol vert en bas */
    SDL_SetRenderDrawColor(renderer, 34, 139, 34, 255);
    SDL_Rect sol = { 0, WINDOW_HEIGHT - 80, WINDOW_WIDTH, 80 };
    SDL_RenderFillRect(renderer, &sol);

    /* Bande sombre derriere le titre */
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 120);
    SDL_Rect bande = { 0, 180, WINDOW_WIDTH, 100 };
    SDL_RenderFillRect(renderer, &bande);

    /* Titre du jeu */
    draw_text_centered(renderer, font,
                       "HUNTER BIRD",
                       200, 255, 220, 50);   /* Jaune */

    /* Instructions */
    draw_text_centered(renderer, font,
                       "Appuie sur ENTREE pour commencer",
                       320, 255, 255, 255);  /* Blanc */

    draw_text_centered(renderer, font,
                       "ECHAP pour quitter",
                       360, 180, 180, 180);  /* Gris  */

    /* Controles */
    draw_text_centered(renderer, font,
                       "Clic gauche = Tirer",
                       430, 150, 220, 255);  /* Bleu clair */
}

/* ================================================
 * 3. MENU_DRAW_LEVELUP
 * Dessine l'ecran de passage au niveau suivant
 *
 * level → le nouveau numero de niveau
 * score → le score actuel du joueur
 * ================================================ */
void menu_draw_levelup(SDL_Renderer *renderer, TTF_Font *font,
                       int level, int score)
{
    char buf[64];

    /* Fond vert fonce */
    SDL_SetRenderDrawColor(renderer, 20, 80, 20, 255);
    SDL_RenderClear(renderer);

    /* Bande sombre au centre */
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 120);
    SDL_Rect bande = { 0, 160, WINDOW_WIDTH, 280 };
    SDL_RenderFillRect(renderer, &bande);

    /* Titre */
    draw_text_centered(renderer, font,
                       "NIVEAU SUIVANT !",
                       180, 100, 255, 100);  /* Vert clair */

    /* Numero du nouveau niveau */
    snprintf(buf, sizeof(buf), "NIVEAU %d", level);
    draw_text_centered(renderer, font,
                       buf, 250, 255, 220, 50); /* Jaune */

    /* Score actuel */
    snprintf(buf, sizeof(buf), "Score : %d", score);
    draw_text_centered(renderer, font,
                       buf, 310, 255, 255, 255); /* Blanc */

    /* Instruction pour continuer */
    draw_text_centered(renderer, font,
                       "Appuie sur ENTREE pour continuer",
                       390, 180, 180, 180);  /* Gris */
}

/* ================================================
 * 4. MENU_DRAW_GAMEOVER
 * Dessine l'ecran Game Over
 *
 * score → le score final du joueur
 * ================================================ */
void menu_draw_gameover(SDL_Renderer *renderer, TTF_Font *font,
                        int score)
{
    char buf[64];

    /* Fond rouge sombre */
    SDL_SetRenderDrawColor(renderer, 80, 0, 0, 255);
    SDL_RenderClear(renderer);

    /* Bande sombre au centre */
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 120);
    SDL_Rect bande = { 0, 160, WINDOW_WIDTH, 280 };
    SDL_RenderFillRect(renderer, &bande);

    /* Titre Game Over */
    draw_text_centered(renderer, font,
                       "GAME OVER",
                       180, 255, 50, 50);    /* Rouge vif */

    /* Score final */
    snprintf(buf, sizeof(buf), "Score final : %d", score);
    draw_text_centered(renderer, font,
                       buf, 270, 255, 220, 50); /* Jaune */

    /* Instructions */
    draw_text_centered(renderer, font,
                       "R pour rejouer",
                       360, 255, 255, 255);  /* Blanc */

    draw_text_centered(renderer, font,
                       "ECHAP pour quitter",
                       400, 180, 180, 180);  /* Gris  */
}