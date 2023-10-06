#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/keyboard.h>
#include <allegro5/allegro_primitives.h>
#include "game.h"

int main() {
    // Inicializa��o do Allegro
    if (!al_init()) {
        fprintf(stderr, "Falha ao inicializar o Allegro.\n");
        return -1;
    }

    al_init_font_addon();
    al_init_ttf_addon();
    al_init_image_addon();
    al_install_keyboard();
    al_install_mouse();
    al_init_primitives_addon();

    // Cria uma janela
    ALLEGRO_DISPLAY* display = al_create_display(800, 600);
    // Verifica se o Display foi criado corretamente
    if (!display) {
        fprintf(stderr, "Falha ao criar janela.\n");
        return -1;
    }

    // Define o t�tulo da janela
    al_set_window_title(display, "ChemiXplorer");

    // Inicializa a fila de eventos
    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
    // Verifica se a fila foi criada corretamente
    if (!event_queue) {
        fprintf(stderr, "Falha ao criar fila de eventos.\n");
        al_destroy_display(display);
        return -1;
    }

    // Cria um timer
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60);

    // Cria uma fila de eventos para o timer
    ALLEGRO_EVENT_QUEUE* timer_queue = al_create_event_queue();

    // Registra os eventos que deseja tratar
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(timer_queue, al_get_timer_event_source(timer));
    al_start_timer(timer);

    int frame = 0; // Vari�vel para controlar o quadro atual da esteira
    int delay_ms = 10; // Atraso de 10 milissegundos entre as renderiza��es

    // Carrega as 24 imagens da esteira em um array de bitmaps
    ALLEGRO_BITMAP* esteira[24];
    for (int i = 0; i < 24; i++) {
        char nomeImg[50];
        snprintf(nomeImg, sizeof(nomeImg), "img/esteira/%d.png", i + 1);
        esteira[i] = al_load_bitmap(nomeImg);
        // Verifica se foi criado corretamente
        if (!esteira[i]) {
            fprintf(stderr, "Falha ao carregar a imagem %d.\n", i + 1);
            for (int j = 0; j < i; j++) {
                al_destroy_bitmap(esteira[j]);
            }
            al_destroy_display(display);
            return -1;
        }
    }

    // Background
    ALLEGRO_BITMAP* background = al_load_bitmap("img/background.png");

    if (!background) {
        fprintf(stderr, "Falha ao carregar o Background");
        return -1;
    }

    // Cientista 
    ALLEGRO_BITMAP* cientista = al_load_bitmap("img/cientista.png");

    if (!cientista) {
        fprintf(stderr, "Falha ao carregar o Cientista");
        return -1;
    }

    // Caldeirao
    ALLEGRO_BITMAP* caldeirao = al_load_bitmap("img/caldeirao.png");

    if (!caldeirao) {
        fprintf(stderr, "Falha ao carregar o Caldeirao");
        return -1;
    }

    int lastPotion = -1; // Valor que n�o representa nenhuma po��o
    int consecutivePotion = 0; // Contador para po��es consecutivas
    srand(time(NULL));
    
    // Carregamento das imagens das po��es
    ALLEGRO_BITMAP* bitmapPotions[6];
    Potion potions[6];
    // Inicializa��o das po��es
    for (int i = 0; i < 6; i++) {
        char nomeImg[50];
        snprintf(nomeImg, sizeof(nomeImg), "img/potion%d.png", i + 1);
        bitmapPotions[i] = al_load_bitmap(nomeImg);
        potions[i].bitmap = bitmapPotions[i];
        if (!potions[i].bitmap) {
            fprintf(stderr, "Falha ao carregar a imagem da po��o %d.\n", i + 1);
            for (int j = 0; j < i; j++) {
                al_destroy_bitmap(bitmapPotions[i]);
            }
            for (int j = 0; j < 24; j++) {
                al_destroy_bitmap(esteira[j]);
            }
            al_destroy_bitmap(background);
            al_destroy_bitmap(cientista);
            al_destroy_bitmap(caldeirao);
            al_destroy_display(display);
            return -1;
        }

        potions[i].x = -150 - i * 150; // Define a posi��o inicial fora da tela e espa�amento entre as po��es
        potions[i].y = 390; // Posi��o vertical
        potions[i].velocidade = 1.0; // Velocidade das po��es
    }

    // Loop principal do jogo
    bool sair = false;
    while (!sair) {
        ALLEGRO_EVENT event;
        if (al_get_next_event(event_queue, &event)) {
            // Verifica se o usu�rio fechou a janela no X
            if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                sair = true;
            }
        }

        // Colocar aqui a l�gica do jogo


       // Explica��o B�sica: a sequ�ncia de eventos � a seguinte:

       // 1 - Voc� limpa a tela.
       // 2 - Desenha a imagem atual da esteira.
       // 3 - Chama al_flip_display para tornar a imagem vis�vel na tela.
       // 4 - Aguarda o atraso(al_rest) para diminuir a velocidade da esteira.
       // 5 - Avan�a para a pr�xima imagem da esteira e repete o processo.
       // Colocar al_flip_display ap�s a renderiza��o da imagem garante que cada imagem seja exibida na tela antes de esperar pelo atraso, criando assim a anima��o desejada da esteira.

        // Limpa a tela
        al_clear_to_color(al_map_rgb(0, 0, 0));

        // Desenha o background
        al_draw_bitmap(background, 0, 0, 0);

        // Desenha o cientista
        al_draw_bitmap(cientista, 250, 270, 0);

        // Desenha o caldeir�o
        al_draw_bitmap(caldeirao, 620, 340, 0);

        // Desenha a imagem atual da esteira
        al_draw_bitmap(esteira[frame], 400, 470, 0);
        al_draw_bitmap(esteira[frame], 300, 470, 0);
        al_draw_bitmap(esteira[frame], 125, 470, 0);
        al_draw_bitmap(esteira[frame], -50, 470, 0);

        // Atualiza a posi��o das po��es e desenha elas
        for (int i = 0; i < 6; i++) {
            potions[i].x += potions[i].velocidade;
            // Se a po��o sair da tela � direita, reposicione todas em ordem aleat�ria
            if (potions[i].x > 600) {
                int randomPotion = rand() % 6;

                // Verifica se a nova po��o � igual � anterior
                if (randomPotion == lastPotion) {
                    consecutivePotion++;
                }
                else {
                    consecutivePotion = 0; // Reinicia o contador se for diferente
                }
                
                // Se tiver 2 po��es consecutivas iguais, escolhe outra aleat�ria
                if (consecutivePotion == 2) {
                    do {
                        randomPotion = rand() % 6;
                    } while (randomPotion == lastPotion);
                    consecutivePotion = 0;
                }

                potions[i].bitmap = bitmapPotions[randomPotion];
                potions[i].x = -290;
                lastPotion = randomPotion; // Atualiza a �ltima po��o usada
            }
            al_draw_bitmap(potions[i].bitmap, potions[i].x, potions[i].y, 0);
        }

        // Atualiza a tela
        al_flip_display();

        // Aguarda o atraso para diminuir a velocidade (dividir por mil pois se trata de milissegundos)
        al_rest(delay_ms / 1000.0);

        // Avan�ando para a pr�xima imagem da esteira - Explica��o:
        // Subtrai 1 do valor atual de frame, isso significa que estamos indo para a imagem anterior na esteira
        // Adiciona 24 ao resultado da etapa anterior. Isso � feito para garantir que o valor seja sempre positivo ou zero. 
        // Se o valor original de frame for 0 e subtrairmos 1, teremos -1, que n�o � um �ndice v�lido para um array. Adicionando 24, tornamos -1 em 23, que � o �ltimo �ndice v�lido. 
        // Finalmente, calcula o resto da divis�o do resultado da etapa anterior por 24. Isso garante que o valor final de frame esteja dentro do intervalo de 0 a 23. Se o valor for maior que 23, ele voltar� para 0, reiniciando o ciclo de imagens da esteira.
        frame = (frame - 1 + 24) % 24;
    }

    // Destruindo recursos
    for (int i = 0; i < 6; i++) {
        al_destroy_bitmap(bitmapPotions[i]);
        potions[i].bitmap = NULL;
    }

    for (int i = 0; i < 24; i++) {
        al_destroy_bitmap(esteira[i]);
    }

    al_destroy_bitmap(background);
    al_destroy_bitmap(cientista);
    al_destroy_bitmap(caldeirao);

    al_destroy_timer(timer);
    al_destroy_event_queue(timer_queue);

    al_destroy_event_queue(event_queue);
    al_destroy_display(display);

    return 0;
}
