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

// Struct para representar uma po  o
typedef struct {
    ALLEGRO_BITMAP* bitmap; // Imagem da po  o
    float x; // Posi  o X da po  o
    float y; // Posi  o Y da po  o
    float velocidade; // Velocidade da po  o
    bool taArrastando; // Flag para verificar se uma po  o   arrastada
    enum TipoPocao id;
} Potion;

typedef struct {
    ALLEGRO_BITMAP* bitmap;
    enum TipoPocao id;
} PotionProperties;

// Estrutura para representar um jogador
typedef struct {
    char name[50];
    int score;
} Player;

