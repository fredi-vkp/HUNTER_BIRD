#ifndef BIRD_H
#define BIRD_H
typedef struct {
    int x;
    int y;
    int vitesse;
} Bird;
void initBird(Bird *b);
void updateBird(Bird *b);
void afficherBird(Bird *b);
#endif