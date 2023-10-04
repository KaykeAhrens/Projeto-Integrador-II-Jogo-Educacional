#pragma once
#include <allegro5/allegro.h>

// Struct para representar uma po��o
typedef struct {
    ALLEGRO_BITMAP* bitmap; // Imagem da po��o
    int x; // Posi��o X da po��o
    int y; // Posi��o Y da po��o
    int velocidade; // Velocidade da po��o
} Potion;