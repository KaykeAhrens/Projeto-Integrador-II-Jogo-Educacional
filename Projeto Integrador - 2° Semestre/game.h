#pragma once
#include <allegro5/allegro.h>

enum TipoPocao {
    NA,
    CL,
    OH,
    AL,
    MG,
    H
};

// Struct para representar uma poção
typedef struct {
    ALLEGRO_BITMAP* bitmap; // Imagem da poção
    float x; // Posição X da poção
    float y; // Posição Y da poção
    float velocidade; // Velocidade da poção
    bool taArrastando; // Flag para verificar se uma poção é arrastada
    enum TipoPocao id;
} Potion;

typedef struct {
    ALLEGRO_BITMAP* bitmap;
    enum TipoPocao id;
} PotionProperties;