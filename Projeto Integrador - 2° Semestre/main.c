#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/keyboard.h>

int main() {
    if (!al_init()) {
        fprintf(stderr, "Falha ao inicializar Allegro.\n");
        return -1;
    }

    al_install_keyboard();
    al_install_mouse();

    // Cria uma janela
    ALLEGRO_DISPLAY* display = al_create_display(800, 600);
    // Verifica se o Display foi criado corretamente
    if (!display) {
        printf("Falha ao criar janela.\n");
        return -1;
    }

    // Define o título da janela
    al_set_window_title(display, "Bem Vindo ao Jogo!");

    // Inicializa a fila de eventos
    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
    // Verifica se a fila foi criada corretamente
    if (!event_queue) {
        printf("Falha ao criar fila de eventos.\n");
        al_destroy_display(display);
        return -1;
    }

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source(display));

    bool sair = false;
    int jogo;
    while (!sair) {
        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);

        if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (event.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                al_destroy_display(display);
                jogo = gamePlay();
                //sair = true;
            }
        }
        else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            break;
        }
    }

    al_destroy_event_queue(event_queue);
    al_destroy_display(display);

    return 0;
}
