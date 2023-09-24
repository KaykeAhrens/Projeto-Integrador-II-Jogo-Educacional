#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/keyboard.h>
#include <allegro5/allegro_primitives.h>

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
    // Verifica se o Display foi criado corretamente
    if (!display) {
        fprintf(stderr, "Falha ao criar janela.\n");
        return -1;
    }

    // Define o título da janela
    al_set_window_title(display, "Jogo Educacional 2D");

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

    // Background
    ALLEGRO_BITMAP* background = al_load_bitmap("img/background.png");

    if (!background) {
        fprintf(stderr, "Falha ao carregar o bg");
        return -1;
    }

    //Cientista 
    ALLEGRO_BITMAP* cientista = al_load_bitmap("img/cientista.png");

    if (!cientista) {
        fprintf(stderr, "Falha ao carregar o cientista");
        return -1;
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

        // Mostra o bg
        al_draw_bitmap(background, 0, 0, 0);

        // Mostra o cientista
        al_draw_bitmap(cientista, 250, 280, 0);

        // Desenha a imagem atual da esteira
        al_draw_bitmap(esteira[frame], 300, 480, 0);
        al_draw_bitmap(esteira[frame], 125, 480, 0);
        al_draw_bitmap(esteira[frame], -50, 480, 0);

        // Atualiza a tela
        al_flip_display();

        // Aguarda o atraso para diminuir a velocidade (dividir por mil pois se trata de milissegundos)
        al_rest(delay_ms / 1000.0);

        // Avançando para a próxima imagem da esteira - Explicação:
        // Subtrai 1 do valor atual de frame, isso significa que estamos indo para a imagem anterior na esteira
        // Adiciona 24 ao resultado da etapa anterior. Isso é feito para garantir que o valor seja sempre positivo ou zero. 
        // Se o valor original de frame for 0 e subtrairmos 1, teremos -1, que não é um índice válido para um array. Adicionando 24, tornamos -1 em 23, que é o último índice válido. 
        // Finalmente, calcula o resto da divisão do resultado da etapa anterior por 24. Isso garante que o valor final de frame esteja dentro do intervalo de 0 a 23. Se o valor for maior que 23, ele voltará para 0, reiniciando o ciclo de imagens da esteira.
        frame = (frame - 1 + 24) % 24;
    }

    for (int i = 0; i < 24; i++) {
        al_destroy_bitmap(esteira[i]);
    }
    
    al_destroy_bitmap(background);
    al_destroy_bitmap(cientista);

    al_destroy_timer(timer);
    al_destroy_event_queue(timer_queue);

    al_destroy_event_queue(event_queue);
    al_destroy_display(display);

    return 0;
}
