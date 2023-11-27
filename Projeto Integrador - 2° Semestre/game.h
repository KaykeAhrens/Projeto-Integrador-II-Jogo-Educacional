#pragma once
#include <allegro5/allegro.h>

enum TipoPocao {
    NA,
    CL,
    OH,
    AL,
    MG,
    H,
    H2,
    O
};

// Struct para representar uma pocao
typedef struct {
    ALLEGRO_BITMAP* bitmap; // Imagem da pocao
    float x; // Posicao X da pocao
    float y; // Posicao Y da pocao
    float velocidade; // Velocidade da pocao
    bool taArrastando; // Flag para verificar se uma pocao foi arrastada
    enum TipoPocao id;
} Potion;

typedef struct {
    ALLEGRO_BITMAP* bitmap;
    enum TipoPocao id;
}PotionProperties;

typedef struct {
    int idElemento1; // Id do elemento 1 do pedido
    int idElemento2; // Id do elemento 2 do pedido
    bool adicionadoElemento1; // Se o elemento 1 foi coletado
    bool adicionadoElemento2; // Se o elemento 2 foi coletado
    const char* textoPedido; // Texto do pedido 
    int indicePedido; // Indice que esta relacionado ao array de pedidos (nomesPedidos[])
    Potion propPedidos;
}Orders;

enum PontodePartida {
    INICIO,
    JOGANDO,
    PLACAR
};

// Estrutura para representar um jogador
typedef struct {
    char name[50];
    int score;
} Player;

