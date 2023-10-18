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
    // Inicialização do Allegro
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

    // Define o título da janela
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
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_start_timer(timer);

    int frame = 0; // Variável para controlar o quadro atual da esteira
    int delay_ms = 1; // Atraso de 1 milissegundo entre as renderizações de todo o jogo
    int frame_counter = 0; // Contador de quadros
    int esteira_delay_frames = 5; // Quantidade de quadros antes de atualizar a esteira (controle de velocidade)

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

    // Cursor do Mouse (Seta)
    ALLEGRO_MOUSE_CURSOR* customCursor = al_create_mouse_cursor(al_load_bitmap("img/simpleCursor.png"), 0, 0);
    if (!customCursor) {
        fprintf(stderr, "Falha ao criar o cursor personalizado.\n");
        return -1;
    }

    // Cursor do Mouse (Mão)
    ALLEGRO_MOUSE_CURSOR* handCustomCursor = al_create_mouse_cursor(al_load_bitmap("img/handCursor.png"), 0, 0);
    if (!customCursor) {
        fprintf(stderr, "Falha ao criar o cursor personalizado.\n");
        return -1;
    }

    int lastPotion = -1; // Valor que não representa nenhuma poção
    int consecutivePotion = 0; // Contador para poções consecutivas
    srand(time(NULL));

    // Carregamento das imagens das poções
    ALLEGRO_BITMAP* bitmapPotions[6];
    Potion potions[6];
    // Inicialização das poções
    for (int i = 0; i < 6; i++) {
        char nomeImg[50];
        snprintf(nomeImg, sizeof(nomeImg), "img/potion%d.png", i + 1);
        bitmapPotions[i] = al_load_bitmap(nomeImg);
        potions[i].bitmap = bitmapPotions[i];
        if (!potions[i].bitmap) {
            fprintf(stderr, "Falha ao carregar a imagem da poção %d.\n", i + 1);
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

        potions[i].x = -150 - i * 150; // Define a posição inicial fora da tela e espaçamento entre as poções
        potions[i].y = 390; // Posição vertical
        potions[i].velocidade = 0.25; // Velocidade das poções
    }

    // Variáveis para o drag and drop
    bool arrastando = false;  // Indica se uma poção está sendo arrastada
    int indice_PocaoArrastada = -1;  // Índice da poção sendo arrastada
    int deslocamentoX, deslocamentoY;  // Deslocamento do mouse em relação ao canto superior esquerdo da poção arrastada
    int lastPosition = 5; // Índice da última poção que será instanciada
    
    // Loop principal do jogo
    bool sair = false;
    while (!sair) {
        ALLEGRO_EVENT event;
        if (al_get_next_event(event_queue, &event)) {
            // Verifica se o usuário fechou a janela no X
            if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                sair = true;
            }
            else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {  // Botão do mouse pressionado
                al_set_mouse_cursor(display, handCustomCursor); // Configura o cursor da mãozinha com o botão pressionado
                if (event.mouse.button == 1) {  // Botão esquerdo do mouse pressionado
                    int mouseX = event.mouse.x;
                    int mouseY = event.mouse.y;

                    // Verifica se o mouse está sobre alguma poção (começando da última para a primeira)
                    for (int i = 5; i >= 0; i--) {
                        if (mouseX >= potions[i].x && mouseX <= (potions[i].x + al_get_bitmap_width(potions[i].bitmap)) &&
                            mouseY >= potions[i].y && mouseY <= (potions[i].y + al_get_bitmap_height(potions[i].bitmap))) {
                            arrastando = true;
                            indice_PocaoArrastada = i;
                            deslocamentoX = mouseX - potions[i].x;
                            deslocamentoY = mouseY - potions[i].y;
                            break;
                        }
                    }
                }
            }
            else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {  // Botão do mouse solto
                al_set_mouse_cursor(display, customCursor);  // Configura o cursor com o botão do mouse solto
                if (event.mouse.button == 1 && arrastando) {  // Botão esquerdo do mouse solto
                    arrastando = false;

                    // Verifica se a poção foi solta dentro do caldeirão
                    if (event.mouse.x >= 620 && event.mouse.x <= (620 + al_get_bitmap_width(caldeirao)) &&
                        event.mouse.y >= 340 && event.mouse.y <= (340 + al_get_bitmap_height(caldeirao))) {
                        // A poção foi solta dentro do caldeirão, colocar a lógica do que acontecerá aqui
                    }
                    else {
                        // A poção foi solta fora do caldeirão, colocar a lógica do que acontecerá aqui
                        potions[indice_PocaoArrastada].y = -1000;
                        potions[indice_PocaoArrastada].velocidade = 0.25;
                    }
                }
            }
            else if (event.type == ALLEGRO_EVENT_MOUSE_AXES && arrastando) {  // Mouse movido enquanto arrasta
                potions[indice_PocaoArrastada].x = event.mouse.x - deslocamentoX;
                potions[indice_PocaoArrastada].y = event.mouse.y - deslocamentoY;
                potions[indice_PocaoArrastada].velocidade = 0;
            }
        }

        // Limpa a tela
        al_clear_to_color(al_map_rgb(0, 0, 0));

        // Desenha o background
        al_draw_bitmap(background, 0, 0, 0);

        // Desenha o cientista
        al_draw_bitmap(cientista, 250, 270, 0);

        // Desenha o caldeirão
        al_draw_bitmap(caldeirao, 620, 340, 0);

        // Desenha a imagem atual da esteira
        al_draw_bitmap(esteira[frame], 400, 470, 0);
        al_draw_bitmap(esteira[frame], 300, 470, 0);
        al_draw_bitmap(esteira[frame], 125, 470, 0);
        al_draw_bitmap(esteira[frame], -50, 470, 0);

        // Atualiza a posição das poções e desenha elas
        for (int i = 0; i < 6; i++) {
            potions[i].x += potions[i].velocidade;
            // Se a poção sair da tela à direita, reposicione todas em ordem aleatória
            if (potions[i].x > 610) {
                int randomPotion = rand() % 6;

                // Verifica se a nova poção é igual à anterior
                if (randomPotion == lastPotion) {
                    consecutivePotion++;
                }
                else {
                    consecutivePotion = 0; // Reinicia o contador se for diferente
                }

                // Se tiver 2 poções consecutivas iguais, escolhe outra aleatória
                if (consecutivePotion == 2) {
                    do {
                        randomPotion = rand() % 6;
                    } while (randomPotion == lastPotion);
                    consecutivePotion = 0;
                }

                potions[i].bitmap = bitmapPotions[randomPotion];
                
                // Define o espaçamento de acordo com a posição da poção arrastada anteriormente
                potions[i].x = potions[lastPosition].x - 150;
                lastPosition = i; 

                potions[i].y = 390;
                lastPotion = randomPotion; 
            }

            al_draw_bitmap(potions[i].bitmap, potions[i].x, potions[i].y, 0);
        }

        // Atualiza a tela
        al_flip_display();

        // Aguarda o atraso para diminuir a velocidade (dividir por mil pois se trata de milissegundos)
        al_rest(delay_ms / 1000.0);

        // Avançando para a próxima imagem da esteira
        frame_counter++;
        if (frame_counter >= esteira_delay_frames) {
            frame = (frame - 1 + 24) % 24;
            frame_counter = 0; // Reinicia o contador de quadros
        }
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
    al_destroy_mouse_cursor(customCursor);
    al_destroy_event_queue(event_queue);
    al_destroy_display(display);

    return 0;
}
