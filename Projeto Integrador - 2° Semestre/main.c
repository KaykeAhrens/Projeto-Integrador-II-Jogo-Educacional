#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>

int main() {

    // Inicializa o Allegro
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
    // Verifica se foi criado corretamente
    if (!display) {
        fprintf(stderr, "Falha ao criar janela.\n");
        return -1;
    }

    // Define o título da janela
    al_set_window_title(display, "Jogo Educacional 2D");

    // Inicializa a fila de eventos
    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
    // Verifica se foi criado corretamente
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

    int frame = 0; // Variável para controlar o quadro atual da esteira
    int delay_ms = 10; // Atraso de 10 milissegundos entre as renderizações

    // Carrega as 24 imagens da esteira em um array de bitmaps
    ALLEGRO_BITMAP* esteira[24];
    for (int i = 0; i < 24; i++) {
        char nomeImg[50];
        snprintf(nomeImg, sizeof(nomeImg), "img/esteira/%d.png", i + 1); //https://pt.strephonsays.com/printf-and-vs-fprintf-10111
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

    // Loop principal do jogo
    bool sair = false;
    while (!sair) {
        ALLEGRO_EVENT event;
        if (al_get_next_event(event_queue, &event)) {
            // Verifica se o usuário fechou a janela no X
            if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                sair = true;
            }
        }

        // Colocar aqui a lógica do jogo


        // Explicação Básica: a sequência de eventos é a seguinte:

        // 1 - Você limpa a tela.
        // 2 - Desenha a imagem atual da esteira.
        // 3 - Chama al_flip_display para tornar a imagem visível na tela.
        // 4 - Aguarda o atraso(al_rest) para diminuir a velocidade da esteira.
        // 5 - Avança para a próxima imagem da esteira e repete o processo.
        // Colocar al_flip_display após a renderização da imagem garante que cada imagem seja exibida na tela antes de esperar pelo atraso, criando assim a animação desejada da esteira.


        // Limpa a tela
        al_clear_to_color(al_map_rgb(0, 0, 0));

        // Desenha a imagem atual da esteira
        al_draw_bitmap(esteira[frame], 300, 450, 0);

        // Atualiza a tela
        al_flip_display();

        // Aguarda o atraso para diminuir a velocidade (dividir por mil pois se trata de milissegundos)
        al_rest(delay_ms / 1000.0);

        // Avança para a próxima imagem da esteira - quando frame atinge 23 (o último índice do vetor esteira), ele volta para 0, reiniciando o ciclo e permitindo que as imagens da esteira sejam renderizadas em um loop contínuo
        frame = (frame + 1) % 24;
    }

    for (int i = 0; i < 24; i++) {
        al_destroy_bitmap(esteira[i]);
    }

    al_destroy_timer(timer);
    al_destroy_event_queue(timer_queue);

    al_destroy_event_queue(event_queue);
    al_destroy_display(display);

    return 0;
}
