#pragma once
#include <allegro5/allegro.h>

// Struct para representar uma po��o
typedef struct {
    ALLEGRO_BITMAP* bitmap; // Imagem da po��o
    float x; // Posi��o X da po��o
    float y; // Posi��o Y da po��o
    float velocidade; // Velocidade da po��o
} Potion;