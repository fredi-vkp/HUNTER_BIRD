/*
 * ================================================
 * HUNTER BIRD
 * Fichier  : src/game.c
 * Role     : Coeur du jeu — init, boucle, rendu
 * Auteur   : Dev A
 * ================================================
 *
 * CE FICHIER CONTIENT 5 PARTIES :
 *   1. init_game()     → Démarre tout
 *   2. handle_events() → Lit clavier et souris
 *   3. update()        → Logique du jeu
 *   4. render()        → Dessine à l'écran
 *   5. game_loop()     → Boucle principale
 *   6. cleanup()       → Libère la mémoire
 * ================================================
 */

#include "game.h"
#include "player.h"
#include "menu.h"
#include <stdio.h>
#include <math.h>

/* ================================================
 * FONCTIONS UTILITAIRES INTERNES
 * static = utilisable uniquement dans ce fichier
 * ================================================ */

/* Charge une image PNG et la convertit en texture SDL2 */
/* Retourne NULL si le fichier est introuvable          */
static SDL_Texture *load_texture(SDL_Renderer *r, const char *path)
{
    SDL_Surface *s = IMG_Load(path);
    if (!s) {
        printf("Image non trouvee : %s\n", path);
        return NULL;
    }
    SDL_Texture *t = SDL_CreateTextureFromSurface(r, s);
    SDL_FreeSurface(s); /* Surface inutile apres conversion */
    return t;
}

/* ================================================
 * PARTIE 1 — INIT_GAME
 * Lance SDL2, cree la fenetre, charge les assets
 * ================================================ */
int init_game(Game *game)
{
    /* --- Demarrer SDL2 --- */
    /* SDL_INIT_VIDEO = fenetre et rendu graphique  */
    /* SDL_INIT_AUDIO = sons et musique             */
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        printf("Erreur SDL_Init : %s\n", SDL_GetError());
        return 0; /* 0 = echec */
    }

    /* --- Demarrer SDL2_image (pour charger PNG) --- */
    if (IMG_Init(IMG_INIT_PNG) == 0) {
        printf("Erreur IMG_Init : %s\n", IMG_GetError());
        return 0;
    }

    /* --- Demarrer SDL2_ttf (pour afficher du texte) --- */
    if (TTF_Init() != 0) {
        printf("Erreur TTF_Init : %s\n", TTF_GetError());
        return 0;
    }

    /* --- Demarrer SDL2_mixer (pour les sons) --- */
    /* 44100 = qualite audio standard (Hz)         */
    /* 2     = stereo (2 canaux)                   */
    /* 2048  = taille du buffer audio              */
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0) {
        printf("Avertissement audio : %s\n", Mix_GetError());
        /* Pas fatal — le jeu continue sans son    */
    }

    /* --- Creer la fenetre --- */
    game->window = SDL_CreateWindow(
        WINDOW_TITLE,           /* Titre affiché dans la barre */
        SDL_WINDOWPOS_CENTERED, /* Centree horizontalement     */
        SDL_WINDOWPOS_CENTERED, /* Centree verticalement       */
        WINDOW_WIDTH,           /* Largeur : 800px             */
        WINDOW_HEIGHT,          /* Hauteur : 600px             */
        0                       /* Pas de flag special         */
    );
    if (!game->window) {
        printf("Erreur fenetre : %s\n", SDL_GetError());
        return 0;
    }

    /* --- Creer le renderer --- */
    /* Le renderer est le moteur qui dessine tout  */
    /* SDL_RENDERER_ACCELERATED = utilise le GPU   */
    /* SDL_RENDERER_PRESENTVSYNC = synchro ecran   */
    game->renderer = SDL_CreateRenderer(
        game->window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (!game->renderer) {
        printf("Erreur renderer : %s\n", SDL_GetError());
        return 0;
    }

    /* --- Cacher le curseur systeme --- */
    /* On dessinera notre propre viseur a la place */
    SDL_ShowCursor(SDL_DISABLE);

    /* --- Charger les images --- */
    /* Si un fichier est absent, load_texture      */
    /* retourne NULL et on dessine des formes      */
    game->background     = load_texture(game->renderer, "assets/images/background.png");
    game->bird_frames[0] = load_texture(game->renderer, "assets/images/bird_1.png");
    game->bird_frames[1] = load_texture(game->renderer, "assets/images/bird_2.png");
    game->bird_dead      = load_texture(game->renderer, "assets/images/bird_dead.png");
    game->crosshair      = load_texture(game->renderer, "assets/images/crosshair.png");

    /* --- Charger la police de texte --- */
    /* Utilisee pour afficher score, vies, etc.    */
    game->font = TTF_OpenFont("assets/font.ttf", 24);
    if (!game->font) {
        printf("Avertissement police : %s\n", TTF_GetError());
        /* Pas fatal — le HUD sera invisible       */
    }

    /* --- Charger les sons --- */
    game->music       = Mix_LoadMUS("assets/sounds/music.mp3");
    game->sound_shoot = Mix_LoadWAV("assets/sounds/shoot.wav");
    game->sound_dead  = Mix_LoadWAV("assets/sounds/bird_dead.wav");

    /* --- Initialiser le joueur --- */
    /* Score = 0, Vies = 3, Ammo = 3           */
    game->player.score      = 0;
    game->player.lives      = 3;
    game->player.ammo       = MAX_AMMO;
    game->player.shot_fired = 0;

    /* --- Initialiser le niveau 1 --- */
    game->level.number        = 1;
    game->level.bird_count    = 3;    /* 3 oiseaux au niveau 1 */
    game->level.bird_speed    = 2.0f; /* Vitesse de depart     */
    game->level.birds_shot    = 0;
    game->level.birds_escaped = 0;

    /* --- Etat initial : on commence au menu --- */
    game->state   = STATE_MENU;
    game->running = 1;

    printf("Jeu initialise avec succes !\n");
    return 1; /* 1 = succes */
}

/* ================================================
 * PARTIE 2 — HANDLE_EVENTS
 * Lit tous les evenements SDL2 de ce frame
 * Un evenement = quelque chose qui s'est passe
 * (clic, touche, fermeture de fenetre...)
 * ================================================ */
static void handle_events(Game *game)
{
    SDL_Event event;

    /* Reset du clic — doit etre remis a 0 chaque frame */
    game->player.shot_fired = 0;

    /* SDL_PollEvent lit un evenement a la fois        */
    /* La boucle vide toute la file d'evenements       */
    while (SDL_PollEvent(&event)) {

        switch (event.type) {

            /* Croix de fermeture de la fenetre */
            case SDL_QUIT:
                game->running = 0;
                break;

            /* Touche clavier enfoncee */
            case SDL_KEYDOWN:

                /* Echap = quitter le jeu */
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    game->running = 0;

                /* Entree = demarrer depuis le menu */
                if (event.key.keysym.sym == SDLK_RETURN
                    && game->state == STATE_MENU) {
                    game->state = STATE_PLAYING;
                    /* Lancer la musique */
                    if (game->music)
                        Mix_PlayMusic(game->music, -1); /* -1 = boucle infinie */
                    /* Spawner les oiseaux du niveau 1 */
                    for (int i = 0; i < game->level.bird_count; i++) {
                        game->birds[i].x     = (float)(rand() % WINDOW_WIDTH);
                        game->birds[i].y     = 50.0f + (rand() % 200);
                        game->birds[i].vx    = game->level.bird_speed;
                        game->birds[i].vy    = 0.0f;
                        game->birds[i].alive = 1;
                        game->birds[i].escaped    = 0;
                        game->birds[i].frame      = 0;
                        game->birds[i].anim_timer = 0.0f;
                        game->birds[i].fall_speed = 0.0f;
                    }
                }

                /* R = rejouer depuis game over */
                if (event.key.keysym.sym == SDLK_r
                    && game->state == STATE_GAME_OVER) {
                    /* Reinitialiser tout */
                    game->player.score      = 0;
                    game->player.lives      = 3;
                    game->player.ammo       = MAX_AMMO;
                    game->level.number      = 1;
                    game->level.bird_count  = 3;
                    game->level.bird_speed  = 2.0f;
                    game->level.birds_shot  = 0;
                    game->state = STATE_MENU;
                }

                /* Entree = continuer depuis level up */
                if (event.key.keysym.sym == SDLK_RETURN
                    && game->state == STATE_LEVEL_UP) {
                    game->state = STATE_PLAYING;
                    /* Respawner les oiseaux du nouveau niveau */
                    for (int i = 0; i < game->level.bird_count; i++) {
                        game->birds[i].x     = (float)(rand() % WINDOW_WIDTH);
                        game->birds[i].y     = 50.0f + (rand() % 200);
                        game->birds[i].vx    = game->level.bird_speed;
                        game->birds[i].alive = 1;
                        game->birds[i].escaped    = 0;
                        game->birds[i].frame      = 0;
                        game->birds[i].anim_timer = 0.0f;
                        game->birds[i].fall_speed = 0.0f;
                    }
                }
                break;

            /* Mouvement de la souris */
            case SDL_MOUSEMOTION:
                /* Met a jour la position du viseur en temps reel */
                game->player.mouse_x = event.motion.x;
                game->player.mouse_y = event.motion.y;
                break;

            /* Clic de la souris */
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT)
                    game->player.shot_fired = 1; /* Tir detecte ! */
                break;
        }
    }
}

/* ================================================
 * PARTIE 3 — UPDATE
 * Met a jour toute la logique du jeu
 * Appelée 60 fois par seconde
 * ================================================ */
static void update(Game *game)
{
    /* On ne met a jour que si on joue */
    if (game->state != STATE_PLAYING) return;

    /* --- Mettre a jour chaque oiseau --- */
    for (int i = 0; i < game->level.bird_count; i++) {
        Bird *b = &game->birds[i];

        if (!b->alive) {
            /* Oiseau mort : il tombe vers le bas */
            b->fall_speed += 0.5f; /* Gravite */
            b->y += b->fall_speed;
            continue; /* Passer a l'oiseau suivant */
        }

        /* Deplacement horizontal */
        b->x += b->vx;

        /* Vol sinusoidal — ondulation naturelle */
        b->y += sinf(b->x * 0.04f) * 1.5f;

        /* Animation des ailes */
        b->anim_timer += 1.0f;
        if (b->anim_timer >= 15.0f) {
            b->frame = 1 - b->frame; /* Alterne 0 et 1 */
            b->anim_timer = 0.0f;
        }

        /* Rebond sur les bords haut/bas */
        if (b->y < 20.0f)               b->vy =  1.0f;
        if (b->y > WINDOW_HEIGHT - 80)  b->vy = -1.0f;
        b->y += b->vy;

        /* Oiseau sorti par les cotes = echappe */
        if (b->x < -60.0f || b->x > WINDOW_WIDTH + 60.0f) {
            b->escaped = 1;
        }
    }

    /* --- Verifier les tirs --- */
    if (game->player.shot_fired && game->player.ammo > 0) {

        game->player.ammo--;

        /* Jouer le son du tir */
        if (game->sound_shoot)
            Mix_PlayChannel(-1, game->sound_shoot, 0);

        /* Verifier si un oiseau est touche */
        for (int i = 0; i < game->level.bird_count; i++) {
            Bird *b = &game->birds[i];
            if (!b->alive) continue;

            /* Distance entre le clic et le centre de l'oiseau */
            float dx   = game->player.mouse_x - b->x;
            float dy   = game->player.mouse_y - b->y;
            float dist = sqrtf(dx * dx + dy * dy);

            if (dist < HIT_RADIUS) {
                /* Oiseau touche ! */
                b->alive      = 0;
                b->fall_speed = 0.0f;
                game->player.score += 10 * game->level.number;
                game->level.birds_shot++;
                if (game->sound_dead)
                    Mix_PlayChannel(-1, game->sound_dead, 0);
            }
        }
    }

    /* --- Recharger si plus de munitions --- */
    if (game->player.ammo <= 0)
        game->player.ammo = MAX_AMMO;

    /* --- Oiseaux echappes : perte de vie --- */
    for (int i = 0; i < game->level.bird_count; i++) {
        if (game->birds[i].escaped) {
            game->birds[i].escaped = 0;
            game->player.lives--;
            /* Respawn de l'oiseau */
            game->birds[i].x     = -30.0f;
            game->birds[i].y     = 50.0f + (rand() % 200);
            game->birds[i].vx    = game->level.bird_speed;
            game->birds[i].alive = 1;
            game->birds[i].fall_speed = 0.0f;
        }
    }

    /* --- Verifier game over --- */
    if (game->player.lives <= 0) {
        game->state = STATE_GAME_OVER;
        Mix_HaltMusic();
        return;
    }

    /* --- Verifier passage de niveau --- */
    if (game->level.birds_shot >= game->level.bird_count) {
        game->level.number++;
        game->level.bird_count  += 1;           /* +1 oiseau      */
        if (game->level.bird_count > MAX_BIRDS)
            game->level.bird_count = MAX_BIRDS;
        game->level.bird_speed  *= 1.2f;        /* +20% vitesse   */
        game->level.birds_shot   = 0;
        game->level.birds_escaped= 0;
        game->player.ammo        = MAX_AMMO;    /* Recharger      */
        game->state = STATE_LEVEL_UP;
    }
}

/* ================================================
 * PARTIE 4 — RENDER
 * Dessine tout a l'ecran
 * Appelée 60 fois par seconde apres update()
 * ================================================ */
static void render(Game *game)
{
    /* Effacer l'ecran avec du noir */
    SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
    SDL_RenderClear(game->renderer);

    switch (game->state) {

        case STATE_MENU:
            /* Fond bleu ciel */
            SDL_SetRenderDrawColor(game->renderer, 30, 100, 200, 255);
            SDL_RenderClear(game->renderer);
            /* Afficher le menu de demarrage */
            menu_draw_start(game->renderer, game->font);
            break;

        case STATE_PLAYING:
            /* Fond d'ecran */
            if (game->background) {
                SDL_RenderCopy(game->renderer, game->background, NULL, NULL);
            } else {
                /* Fond bleu par defaut si pas d'image */
                SDL_SetRenderDrawColor(game->renderer, 30, 100, 200, 255);
                SDL_RenderClear(game->renderer);
                /* Sol vert */
                SDL_SetRenderDrawColor(game->renderer, 34, 139, 34, 255);
                SDL_Rect sol = {0, WINDOW_HEIGHT - 60, WINDOW_WIDTH, 60};
                SDL_RenderFillRect(game->renderer, &sol);
            }

            /* Dessiner chaque oiseau */
            for (int i = 0; i < game->level.bird_count; i++) {
                Bird *b = &game->birds[i];
                SDL_Rect dst = {
                    (int)b->x - 25,
                    (int)b->y - 20,
                    50, 40
                };

                if (!b->alive) {
                    /* Oiseau mort : rouge */
                    if (game->bird_dead) {
                        SDL_RenderCopy(game->renderer, game->bird_dead, NULL, &dst);
                    } else {
                        SDL_SetRenderDrawColor(game->renderer, 200, 50, 50, 200);
                        SDL_RenderFillRect(game->renderer, &dst);
                    }
                } else {
                    /* Oiseau vivant : sprite ou forme */
                    SDL_Texture *tex = game->bird_frames[b->frame];
                    if (tex) {
                        SDL_RendererFlip flip = (b->vx < 0)
                            ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
                        SDL_RenderCopyEx(game->renderer, tex,
                            NULL, &dst, 0, NULL, flip);
                    } else {
                        /* Corps */
                        SDL_SetRenderDrawColor(game->renderer, 80, 60, 200, 255);
                        SDL_RenderFillRect(game->renderer, &dst);
                        /* Aile animee */
                        SDL_Rect aile = {
                            dst.x + 10,
                            b->frame == 0 ? dst.y - 8 : dst.y + 18,
                            30, 8
                        };
                        SDL_SetRenderDrawColor(game->renderer, 50, 40, 170, 255);
                        SDL_RenderFillRect(game->renderer, &aile);
                    }
                }
            }

            /* Dessiner le viseur via player.c */
            player_draw_crosshair(&game->player, game->renderer, game->crosshair);

            /* Dessiner le HUD (score, vies, ammo) */
            {
                /* Barre noire semi-transparente en haut */
                SDL_SetRenderDrawBlendMode(game->renderer, SDL_BLENDMODE_BLEND);
                SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 140);
                SDL_Rect barre = {0, 0, WINDOW_WIDTH, 44};
                SDL_RenderFillRect(game->renderer, &barre);

                /* Munitions : cercles jaunes/gris */
                for (int i = 0; i < MAX_AMMO; i++) {
                    if (i < game->player.ammo)
                        SDL_SetRenderDrawColor(game->renderer, 255, 200, 0, 255);
                    else
                        SDL_SetRenderDrawColor(game->renderer, 80, 80, 80, 255);
                    SDL_Rect balle = {20 + i * 30, 13, 20, 18};
                    SDL_RenderFillRect(game->renderer, &balle);
                }

                /* Vies : carres rouges */
                for (int i = 0; i < game->player.lives; i++) {
                    SDL_SetRenderDrawColor(game->renderer, 220, 50, 50, 255);
                    SDL_Rect vie = {700 + i * 30, 12, 22, 20};
                    SDL_RenderFillRect(game->renderer, &vie);
                }
            }
            break;

        case STATE_LEVEL_UP:
            SDL_SetRenderDrawColor(game->renderer, 20, 80, 20, 255);
            SDL_RenderClear(game->renderer);
            menu_draw_levelup(game->renderer, game->font,
                              game->level.number, game->player.score);
            break;

        case STATE_GAME_OVER:
            SDL_SetRenderDrawColor(game->renderer, 80, 0, 0, 255);
            SDL_RenderClear(game->renderer);
            menu_draw_gameover(game->renderer, game->font, game->player.score);
            break;
    }

    /* Afficher ce qu'on vient de dessiner */
    SDL_RenderPresent(game->renderer);
}

/* ================================================
 * PARTIE 5 — GAME_LOOP
 * Boucle principale — tourne 60 fois par seconde
 *
 * Chaque tour = 1 frame = 1/60 de seconde
 *   1. Lire les inputs
 *   2. Mettre a jour la logique
 *   3. Dessiner
 *   4. Attendre si necessaire (cap FPS)
 * ================================================ */
void game_loop(Game *game)
{
    Uint32 frame_start; /* Temps debut de frame en ms */
    Uint32 frame_time;  /* Duree de la frame en ms    */

    while (game->running) {

        frame_start = SDL_GetTicks(); /* Demarre le chrono */

        handle_events(game); /* 1. Inputs  */
        update(game);        /* 2. Logique */
        render(game);        /* 3. Rendu   */

        /* 4. Cap FPS : si la frame a ete trop rapide, on attend */
        frame_time = SDL_GetTicks() - frame_start;
        if (frame_time < FRAME_DELAY)
            SDL_Delay(FRAME_DELAY - frame_time);
    }
}

/* ================================================
 * PARTIE 6 — CLEANUP
 * Libere toute la memoire avant de quitter
 * TOUJOURS liberer dans l'ordre inverse de creation
 * ================================================ */
void cleanup(Game *game)
{
    /* Sons */
    if (game->sound_shoot)   Mix_FreeChunk(game->sound_shoot);
    if (game->sound_dead)    Mix_FreeChunk(game->sound_dead);
    if (game->music)         Mix_FreeMusic(game->music);

    /* Textures */
    if (game->background)    SDL_DestroyTexture(game->background);
    if (game->bird_frames[0])SDL_DestroyTexture(game->bird_frames[0]);
    if (game->bird_frames[1])SDL_DestroyTexture(game->bird_frames[1]);
    if (game->bird_dead)     SDL_DestroyTexture(game->bird_dead);
    if (game->crosshair)     SDL_DestroyTexture(game->crosshair);

    /* Police */
    if (game->font)          TTF_CloseFont(game->font);

    /* SDL2 */
    if (game->renderer)      SDL_DestroyRenderer(game->renderer);
    if (game->window)        SDL_DestroyWindow(game->window);

    /* Fermer toutes les sous-bibliotheques */
    Mix_CloseAudio();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    printf("Nettoyage termine.\n");
}