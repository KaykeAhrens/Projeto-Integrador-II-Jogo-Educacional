#pragma once
#include <allegro5/allegro.h>

// Struct para representar uma poção
typedef struct {
    ALLEGRO_BITMAP* bitmap; // Imagem da poção
    int x; // Posição X da poção
    int y; // Posição Y da poção
    int velocidade; // Velocidade da poção
} Potion;