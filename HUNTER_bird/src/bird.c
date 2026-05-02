#include <stdio.h>
#include "bird.h"
void initBird(Bird *b){
    b->x = 100;
    b->y = 200;
    b->vitesse = 0;
}
void updateBird(Bird *b){
    b->vitesse = b->vitesse + 1;
    b->y = b->y + b->vitesse;
}
void afficherBird(Bird * b){
    printf("position y : %d\n", b->y);
}