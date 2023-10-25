#pragma once
#include <allegro5/allegro.h>

// Struct para representar uma poção
typedef struct {
    ALLEGRO_BITMAP* bitmap; // Imagem da poção
    float x; // Posição X da poção
    float y; // Posição Y da poção
    float velocidade; // Velocidade da poção
    bool taArrastando; // Flag para verificar se uma poção é arrastada
} Potion;