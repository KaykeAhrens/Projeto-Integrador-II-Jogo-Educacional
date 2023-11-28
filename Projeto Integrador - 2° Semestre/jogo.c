#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/keyboard.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <string.h>
#include "game.h"

// Função de ordenação (Insertion Sort) para ordenar o vetor de jogadores em ordem decrescente de pontuação
void insertionSort(Player arr[], int n) {
    int i, j;
    Player key;

    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;

        while (j >= 0 && arr[j].score < key.score) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

Player players[7];
void placar(Player* player, int numPlayers, ALLEGRO_BITMAP* background, ALLEGRO_BITMAP* shadow, ALLEGRO_BITMAP* logo,
    ALLEGRO_FONT* fonte2, ALLEGRO_FONT* fonte3, ALLEGRO_DISPLAY* display) {
    if (numPlayers < 10) {
        players[numPlayers - 1] = *player;
        insertionSort(players, numPlayers);

        // Exibir placar após cada jogador
        al_draw_bitmap(background, 0, 0, 0); // Desenha a imagem de fundo

        for (int i = 0; i < numPlayers; ++i) {
            drawLosango(200, 165 + i * (40 + 10), 40, i + 1, fonte3);
            al_draw_textf(fonte2, al_map_rgb(255, 255, 255), 360, 155 + i * (40 + 10), ALLEGRO_ALIGN_LEFT, " %s", players[i].name);
            al_draw_textf(fonte2, al_map_rgb(255, 255, 255), 565, 155 + i * (40 + 10), ALLEGRO_ALIGN_LEFT, " %d pts", players[i].score);
            drawShadow(255, 190 + i * (40 + 10), shadow);
        }

        al_flip_display(); // Atualiza o display

        // Exibir opções de voltar ou sair após cada jogador
        displayOptions(fonte2);
        al_rest(5);

        ALLEGRO_KEYBOARD_STATE keyState;
        while (true) {
            al_get_keyboard_state(&keyState);
            if (al_key_down(&keyState, ALLEGRO_KEY_V)) {
                // Voltar para o início do loop para o próximo jogador
                al_destroy_display(display);
                inicio(player);
            }
            else if (al_key_down(&keyState, ALLEGRO_KEY_S)) {
                al_destroy_display(display);
                al_destroy_bitmap(logo);
                return;
            }
        }
    }

    al_flip_display();
    al_rest(5);

    al_destroy_bitmap(logo);
    al_destroy_bitmap(shadow);
    al_destroy_display(display);

    return;
}

int pegarIndexPocao(char* nomeImagem) {
    char* token = NULL;
    char* imageSplit1 = strtok_s(nomeImagem, ".", &token);
    char* imageSplit2 = strtok_s(imageSplit1, "n", &token);

    int indexPotion = atoi(token);
    return indexPotion;
}

int gamePlay(ALLEGRO_DISPLAY* display, Player* player, int numPlayers) {
    // Inicializa  o do Allegro
    if (!al_init()) {
        printf("Falha ao inicializar o Allegro.\n");
        return -1;
    }

    al_init_font_addon();
    al_init_ttf_addon();
    al_init_acodec_addon();
    al_init_image_addon();
    al_install_keyboard();
    al_install_mouse();
    al_install_audio();
    al_init_primitives_addon();

    // Verifica se o Display foi criado corretamente
    if (!display) {
        printf("Falha ao criar janela.\n");
        return -1;
    }

    // Define o t tulo da janela
    al_set_window_title(display, "ChemiXplorer");

    // Inicializa a fila de eventos
    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
    // Verifica se a fila foi criada corretamente
    if (!event_queue) {
        printf("Falha ao criar fila de eventos.\n");
        al_destroy_display(display);
        return -1;
    }

    // Cria um timer
    ALLEGRO_TIMER* timer = al_create_timer(1.0);

    // Cria uma fonte
    ALLEGRO_FONT* fonte = al_load_ttf_font("fontes/Exo-Regular.ttf", 20, 0);
    ALLEGRO_FONT* fonte2 = al_load_ttf_font("fontes/Exo-ExtraLight.ttf", 20, 0);
    ALLEGRO_FONT* fonte3 = al_load_ttf_font("fontes/Exo-SemiBold.ttf", 20, 0);

    // Cria as m sicas
    ALLEGRO_SAMPLE* msc_timer = al_load_sample("msc/mscTemporizador.mp3");
    ALLEGRO_SAMPLE* msc_game = al_load_sample("msc/mscJogo.mp3");
    al_reserve_samples(2);

    // Carrega a imagem da logo do jogo
    ALLEGRO_BITMAP* logo = al_load_bitmap("img/logo.png");
    if (!logo) {
        fprintf(stderr, "Falha ao carregar a imagem da logo!\n");
        return -1;

    }

    // Carrega a imagem de fundo do placar
    ALLEGRO_BITMAP* bgPlacar = al_load_bitmap("img/fundo_placar.png");
    if (!bgPlacar) {
        fprintf(stderr, "Falha ao carregar a imagem de fundo!\n");
        return -1;
    }

    // Carrega e desenha a imagem de fundo da tela BemVindo
    ALLEGRO_BITMAP* bemvindo = al_load_bitmap("img/fundo_bemvindo.png");
    if (!bgPlacar) {
        fprintf(stderr, "Falha ao carregar a imagem de fundo!\n");
        return -1;
    }

    // Carrega a imagem de sombra
    ALLEGRO_BITMAP* shadow = al_load_bitmap("img/sombra.png");
    if (!shadow) {
        fprintf(stderr, "Falha ao carregar a imagem de sombra!\n");
        return -1;
    }

    // Cria uma fila de eventos para o timer
    ALLEGRO_EVENT_QUEUE* timer_queue = al_create_event_queue();

    // Registra os eventos que deseja tratar
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(timer_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_mouse_event_source());

    // Temporizador
    float tempoAntigo = al_get_time();
    int segundos = 60;
    al_start_timer(timer);

    int frame = 0; // Vari vel para controlar o quadro atual da esteira
    int delay_ms = 1; // Atraso de 1 milissegundo entre as renderiza  es de todo o jogo
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
            printf("Falha ao carregar a imagem %d.\n", i + 1);
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
        printf("Falha ao carregar o Background");
        return -1;
    }

    // Cientista 
    ALLEGRO_BITMAP* cientista = al_load_bitmap("img/cientista.png");

    if (!cientista) {
        printf("Falha ao carregar o Cientista");
        return -1;
    }

    // Caldeirao
    ALLEGRO_BITMAP* caldeirao = al_load_bitmap("img/caldeirao.png");

    if (!caldeirao) {
        printf("Falha ao carregar o Caldeirao");
        return -1;
    }

    // Varal
    ALLEGRO_BITMAP* varal = al_load_bitmap("img/varal.png");

    // Cron metro
    ALLEGRO_BITMAP* cronometro = al_load_bitmap("img/cronometro.png");

    // Cursor do Mouse (Seta)
    ALLEGRO_MOUSE_CURSOR* customCursor = al_create_mouse_cursor(al_load_bitmap("img/simpleCursor.png"), 0, 0);
    if (!customCursor) {
        printf("Falha ao criar o cursor da seta personalizada.\n");
        return -1;
    }

    // Cursor do Mouse (M o)
    ALLEGRO_MOUSE_CURSOR* handCustomCursor = al_create_mouse_cursor(al_load_bitmap("img/handCursor.png"), 0, 0);
    if (!handCustomCursor) {
        printf("Falha ao criar o cursor da m o personalizado.\n");
        return -1;
    }

    int lastPotion = -1; // Valor que n o representa nenhuma po  o
    int consecutivePotion = 0; // Contador para po  es consecutivas
    srand(time(NULL));

    // Carregamento das imagens das po  es
    PotionProperties bitmapPotions[6];
    Potion potions[6];
    // Inicializa  o das po  es
    for (int i = 0; i < 6; i++) {
        char nomeImg[50];
        snprintf(nomeImg, sizeof(nomeImg), "img/potion%d.png", i + 1);
        bitmapPotions[i].bitmap = al_load_bitmap(nomeImg);
        // Pega o index da po  o em rela  o ao seu endere o de imagem
        // Subtrai 1 do resultado pois o array come a em 0 e atribui ao id do bitmap da po  o
        bitmapPotions[i].id = pegarIndexPocao(&nomeImg) - 1;
        potions[i].bitmap = bitmapPotions[i].bitmap;
        potions[i].id = bitmapPotions[i].id;

        if (!potions[i].bitmap) {
            printf("Falha ao carregar a imagem da po  o %d.\n", i + 1);
            for (int j = 0; j < i; j++) {
                al_destroy_bitmap(bitmapPotions[i].bitmap);
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

        potions[i].x = -150 - i * 150; // Define a posi  o inicial fora da tela e espa amento entre as po  es
        potions[i].y = 390; // Posi  o vertical
        potions[i].velocidade = 0.25; // Velocidade das po  es
        potions[i].taArrastando = false;
    }

    // Carregamento dos Pedidos
    ALLEGRO_BITMAP* bitmapOrders[5];
    Potion orders[5];

    // Inicializando os Pedidos
    for (int i = 0; i < 5; i++) {
        char nomeImg[50];
        snprintf(nomeImg, sizeof(nomeImg), "img/pedido%d.png", i + 1);
        bitmapOrders[i] = al_load_bitmap(nomeImg);
        orders[i].bitmap = bitmapOrders[i];
        if (!orders[i].bitmap) {
            printf("Falha ao carregar o pedido %d\n", i + 1);
            for (int j = 0; j < i; j++) {
                al_destroy_bitmap(bitmapOrders[j]);
            }
            return -1;
        }

        orders[i].x = 300 - i * 70;
        orders[i].y = 18;
    }

    int ordem_indices[5] = { 0, 1, 2, 3, 4 };

    for (int i = 4; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = ordem_indices[i];
        ordem_indices[i] = ordem_indices[j];
        ordem_indices[j] = temp;
    }

    // Mapeamento entre  ndices de po  es e tipos correspondentes
    enum TipoPocao tipoPorIndice[6] = { NA, CL, OH, AL, MG, H };

    // Vari veis para o drag and drop
    bool arrastando = false;  // Indica se uma po  o est  sendo arrastada
    int indice_PocaoArrastada = -1;  //  ndice da po  o sendo arrastada
    int deslocamentoX, deslocamentoY;  // Deslocamento do mouse em rela  o ao canto superior esquerdo da po  o arrastada
    int lastPosition = 5; //  ndice da  ltima po  o que ser  instanciada
    bool taArrastando = false;

    int pontuacao = 0; // Defina a pontua  o inicial

    // Loop principal do jogo
    bool sair = false;
    al_play_sample(msc_game, 0.5, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, 0);
    while (!sair) {
        float tempoAtual = al_get_time();
        if (tempoAtual - tempoAntigo >= 1) {
            tempoAntigo = tempoAtual;
            segundos--;
        }

        ALLEGRO_EVENT event;
        if (al_get_next_event(event_queue, &event)) {
            // Verifica se o usu rio fechou a janela no X
            if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                sair = true;
            }
            else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {  // Bot o do mouse pressionado
                al_set_mouse_cursor(display, handCustomCursor); // Configura o cursor da m ozinha com o bot o pressionado
                if (event.mouse.button == 1) {  // Bot o esquerdo do mouse pressionado
                    int mouseX = event.mouse.x;
                    int mouseY = event.mouse.y;

                    // Verifica se o mouse est  sobre alguma po  o (come ando da  ltima para a primeira)
                    for (int i = 5; i >= 0; i--) {
                        if (mouseX >= potions[i].x && mouseX <= (potions[i].x + al_get_bitmap_width(potions[i].bitmap)) &&
                            mouseY >= potions[i].y && mouseY <= (potions[i].y + al_get_bitmap_height(potions[i].bitmap))) {
                            arrastando = true;
                            potions[i].taArrastando = true;
                            indice_PocaoArrastada = i;
                            deslocamentoX = mouseX - potions[i].x;
                            deslocamentoY = mouseY - potions[i].y;
                            break;
                        }
                    }
                }
            }
            else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {  // Bot o do mouse solto

                al_set_mouse_cursor(display, customCursor);  // Configura o cursor com o bot o do mouse solto
                if (event.mouse.button == 1 && arrastando) {  // Bot o esquerdo do mouse solto
                    arrastando = false;

                    potions[indice_PocaoArrastada].taArrastando = false;
                    potions[indice_PocaoArrastada].velocidade = 0.25;
                    // Verifica se a po  o foi solta dentro do caldeir o
                    if (event.mouse.x >= 620 && event.mouse.x <= (620 + al_get_bitmap_width(caldeirao)) &&
                        event.mouse.y >= 340 && event.mouse.y <= (340 + al_get_bitmap_height(caldeirao))) {
                        // A po  o foi solta dentro do caldeir o, colocar a l gica do que acontecer  aqui
                        int tipoPocaoArrastada = tipoPorIndice[potions[indice_PocaoArrastada].id];
                        int tipoPedidoAtual = tipoPorIndice[ordem_indices[0]];

                        printf("Tipo da po  o arrastada: %d\n", tipoPocaoArrastada);
                        printf("Tipo do pedido atual: %d\n", tipoPedidoAtual);

                        if (tipoPocaoArrastada == tipoPedidoAtual) {
                            pontuacao += 10;
                        }
                        else {
                            pontuacao -= 10;
                        }
                    }
                    else {
                        // A po  o foi solta fora do caldeir o, colocar a l gica do que acontecer  aqui
                        potions[indice_PocaoArrastada].y = -1000;
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

        // Desenha o caldeir o
        al_draw_bitmap(caldeirao, 620, 340, 0);

        // Desenha o varal 
        al_draw_bitmap(varal, -13, 20, 0);

        // Desenha a imagem atual da esteira
        al_draw_bitmap(esteira[frame], 400, 470, 0);
        al_draw_bitmap(esteira[frame], 300, 470, 0);
        al_draw_bitmap(esteira[frame], 125, 470, 0);
        al_draw_bitmap(esteira[frame], -50, 470, 0);

        // Atualiza a posi  o das po  es e desenha elas
        for (int i = 0; i < 6; i++) {
            potions[i].x += potions[i].velocidade;
            // Se a po  o sair da tela   direita, reposicione todas em ordem aleat ria
            if (potions[i].x > 550 && !potions[i].taArrastando) {
                int randomPotion = rand() % 6;

                // Verifica se a nova po  o   igual   anterior
                if (randomPotion == lastPotion) {
                    consecutivePotion++;
                }
                else {
                    consecutivePotion = 0; // Reinicia o contador se for diferente
                }

                // Se tiver 2 po  es consecutivas iguais, escolhe outra aleat ria
                if (consecutivePotion == 2) {
                    do {
                        randomPotion = rand() % 6;
                    } while (randomPotion == lastPotion);
                    consecutivePotion = 0;
                }

                //relacionando aquele numero randomico com a po  o e o id dela
                potions[i].bitmap = bitmapPotions[randomPotion].bitmap;
                potions[i].id = bitmapPotions[randomPotion].id;

                // Define o espa amento de acordo com a posi  o da po  o arrastada anteriormente
                potions[i].x = potions[lastPosition].x - 150;
                lastPosition = i;

                potions[i].y = 390;
                lastPotion = randomPotion;
            }

            al_draw_bitmap(potions[i].bitmap, potions[i].x, potions[i].y, 0);
        }

        // Posiciona os pedidos na tela de acordo com a ordem embaralhada
        for (int i = 0; i < 5; i++) {
            al_draw_bitmap(bitmapOrders[ordem_indices[i]], orders[i].x, orders[i].y, 0);
        }

        // Temporizador
        al_draw_bitmap(cronometro, 480, 35, 0);
        al_draw_text(fonte, al_map_rgb(255, 255, 255), 600, 42, ALLEGRO_ALIGN_CENTRE, "Tempo Restante - ");
        // Formata os minutos e segundos com dois d gitos
        int minutos = segundos / 60;
        int segundos_restantes = segundos % 60;
        al_draw_textf(fonte, al_map_rgb(255, 255, 255), 710, 42, ALLEGRO_ALIGN_CENTRE, "%02d:%02d", minutos, segundos_restantes);

        if (segundos <= 10) {
            al_stop_sample(msc_game);
            al_play_sample(msc_timer, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
            al_draw_textf(fonte, al_map_rgb(255, 0, 0), 710, 42, ALLEGRO_ALIGN_CENTRE, "%02d:%02d", minutos, segundos_restantes);
        }
        if (segundos == 0) {
            al_destroy_sample(msc_game);
            al_destroy_sample(msc_timer);

            player->score = pontuacao;
            placar(player, numPlayers, bgPlacar, shadow, logo, fonte2, fonte3, display);
        }

        al_draw_textf(fonte, al_map_rgb(255, 255, 255), 605, 82, ALLEGRO_ALIGN_RIGHT, "Pontos: %d", pontuacao);

        // Atualiza a tela
        al_flip_display();

        // Aguarda o atraso para diminuir a velocidade (dividir por mil pois se trata de milissegundos)
        al_rest(delay_ms / 1000.0);

        // Avan ando para a pr xima imagem da esteira
        frame_counter++;
        if (frame_counter >= esteira_delay_frames) {
            frame = (frame - 1 + 24) % 24;
            frame_counter = 0; // Reinicia o contador de quadros
        }
    }

    // Destruindo recursos
    for (int i = 0; i < 6; i++) {
        al_destroy_bitmap(bitmapPotions[i].bitmap);
        potions[i].bitmap = NULL;
    }

    for (int i = 0; i < 5; i++) {
        al_destroy_bitmap(bitmapOrders[i]);
        orders[i].bitmap = NULL;
    }

    for (int i = 0; i < 24; i++) {
        al_destroy_bitmap(esteira[i]);
    }

    al_destroy_bitmap(background);
    al_destroy_bitmap(cientista);
    al_destroy_bitmap(caldeirao);
    al_destroy_bitmap(varal);
    al_destroy_bitmap(cronometro);

    al_destroy_timer(timer);
    al_destroy_font(fonte);
    al_destroy_sample(msc_timer);
    al_destroy_sample(msc_game);
    al_destroy_event_queue(timer_queue);
    al_destroy_mouse_cursor(handCustomCursor);
    al_destroy_mouse_cursor(customCursor);
    al_destroy_event_queue(event_queue);
    //al_destroy_display(display);

    return 0;
}