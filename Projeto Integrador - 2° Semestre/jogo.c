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

// Fun??o de ordena??o (Insertion Sort) para ordenar o vetor de jogadores em ordem decrescente de pontua??o
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

Player players[10];

void placar(Player* player, int numPlayers, ALLEGRO_BITMAP* bg,
    ALLEGRO_BITMAP* shadow, ALLEGRO_BITMAP* logo,
    ALLEGRO_FONT* fonte2, ALLEGRO_FONT* fonte3, ALLEGRO_DISPLAY* display)
{
    if (numPlayers < 10)
    {
        players[numPlayers - 1] = *(player);
        insertionSort(players, numPlayers);

        al_draw_bitmap(bg, 0, 0, 0); // Desenha a imagem de fundo
        for (int i = 0; i < numPlayers; ++i) {
            drawLosango(200, 165 + i * (40 + 10), 40, i + 1, fonte3);
            al_draw_textf(fonte2, al_map_rgb(255, 255, 255), 360, 155 + i * (40 + 10), ALLEGRO_ALIGN_LEFT, " %s", players[i].name);
            al_draw_textf(fonte2, al_map_rgb(255, 255, 255), 565, 155 + i * (40 + 10), ALLEGRO_ALIGN_LEFT, " %d pts", players[i].score);
            drawShadow(255, 190 + i * (40 + 10), shadow);
        }

        al_flip_display(); // Atualiza o display

        // Exibir op??es de voltar ou sair ap?s cada jogador
        displayOptions(fonte2);
        al_rest(10);

        ALLEGRO_KEYBOARD_STATE keyState;
        while (true) {
            al_get_keyboard_state(&keyState);
            if (al_key_down(&keyState, ALLEGRO_KEY_V)) {
                // Voltar para o in?cio do loop para o pr?ximo jogador
                al_destroy_display(display);
                inicio();
                //return;
            }
            else if (al_key_down(&keyState, ALLEGRO_KEY_S)) {
                al_destroy_display(display);
                al_destroy_bitmap(logo);
                return;
            }
        }
    }

    return;
}

// Fun??o do Placar
//int placar(Player players[], int numPlayers, int pontuacao, ALLEGRO_BITMAP* background, ALLEGRO_BITMAP* shadow, ALLEGRO_BITMAP* logo,
//    ALLEGRO_FONT* fonte2, ALLEGRO_FONT* fonte3, ALLEGRO_DISPLAY* display, char* nomeJogador) {
//    while (numPlayers < 10) {
//        // Controle de pontua??o para cada jogador 
//        players[numPlayers - 1].score = pontuacao;
//        insertionSort(players, numPlayers);
//        //*players[numPlayers - 1].name = nomeJogador;
//        //char jogadores[10][50];
//        int pos = 0;
//
//        // Exibir placar ap?s cada jogador
//        al_draw_bitmap(background, 0, 0, 0); // Desenha a imagem de fundo
//        for (int i = 0; i < numPlayers; ++i) {
//            printf("%s\n", players[i].name);
//            //jogadores[pos][50] = nomeJogador;
//
//            drawLosango(200, 165 + i * (40 + 10), 40, i + 1, fonte3);
//            al_draw_textf(fonte2, al_map_rgb(255, 255, 255), 360, 155 + i * (40 + 10), ALLEGRO_ALIGN_LEFT, " %s", nomeJogador);
//            al_draw_textf(fonte2, al_map_rgb(255, 255, 255), 565, 155 + i * (40 + 10), ALLEGRO_ALIGN_LEFT, " %d pts", players[i].score);
//            drawShadow(255, 190 + i * (40 + 10), shadow);
//            pos++;
//        }
//
//        al_flip_display(); // Atualiza o display
//
//        // Exibir op??es de voltar ou sair ap?s cada jogador
//        displayOptions(fonte2);
//        al_rest(2);
//
//        ALLEGRO_KEYBOARD_STATE keyState;
//        while (true) {
//            al_get_keyboard_state(&keyState);
//            if (al_key_down(&keyState, ALLEGRO_KEY_V)) {
//                // Voltar para o in?cio do loop para o pr?ximo jogador
//                al_destroy_display(display);
//                inicio();
//                //return;
//            }
//            else if (al_key_down(&keyState, ALLEGRO_KEY_S)) {
//                al_destroy_display(display);
//                al_destroy_bitmap(logo);
//                return -1;
//            }
//        }
//    }
//
//    // Ordena o vetor de jogadores em ordem decrescente de pontua??o usando Insertion Sort
//    insertionSort(players, numPlayers);
//
//    // Exibir placar final
//    al_draw_bitmap(background, 0, 0, 0); // Desenha a imagem de fundo
//    for (int i = 0; i < numPlayers; ++i) {
//        drawLosango(200, 165 + i * (40 + 10), 40, i + 1, fonte3);
//        al_draw_textf(fonte2, al_map_rgb(255, 255, 255), 360, 155 + i * (40 + 10), ALLEGRO_ALIGN_LEFT, " %s", players[i].name);
//        al_draw_textf(fonte2, al_map_rgb(255, 255, 255), 565, 155 + i * (40 + 10), ALLEGRO_ALIGN_LEFT, " %d pts", players[i].score);
//        drawShadow(255, 190 + i * (40 + 10), shadow);
//    }
//
//    al_flip_display();
//    al_rest(5); // Aguarda 5 segundos para exibir o placar final antes de encerrar
//
//    al_destroy_bitmap(logo); // Libera a mem?ria alocada para a imagem da logo
//    al_destroy_bitmap(shadow);// Libera a mem?ria alocada para a imagem de sombra
//    al_destroy_display(display); // Destr?i o display
//    return 0;
//}

const char* nomesPedidos[5] = { "NaOH", "Agua", "Sal", "MgOH", "HCl" };
bool nomeUsado[5] = { false, false, false, false, false };
int ultimoIndiceEscolhido = -1;

int pegarIndexPocao(char* nomeImagem) {
    char* token = NULL;
    char* imageSplit1 = strtok_s(nomeImagem, ".", &token);
    char* imageSplit2 = strtok_s(imageSplit1, "n", &token);

    int indexPotion = atoi(token);
    return indexPotion;
}

const char* gerarNovoTexto(int indicePedido) {
    int indice;
    do {
        indice = rand() % 5;
    } while (indice == ultimoIndiceEscolhido || indice == indicePedido);

    ultimoIndiceEscolhido = indice;
    return nomesPedidos[indice];
}

void pegarFormulaPedido(int indexPedido, Orders* pedido)
{
    switch (indexPedido)
    {
    case 0: // NaOH
        pedido->idElemento1 = 0; // Na
        pedido->idElemento2 = 2; // OH
        break;
    case 1: // H2O
        pedido->idElemento1 = 6; // H2
        pedido->idElemento2 = 7; // O
        break;
    case 2: // Sal - NaCl
        pedido->idElemento1 = 0; // Na
        pedido->idElemento2 = 1; // Cl
        break;
    case 3: // MgOH
        pedido->idElemento1 = 4; // Mg
        pedido->idElemento2 = 2; // OH
        break;
    case 4: // HCl
        pedido->idElemento1 = 5; // H
        pedido->idElemento2 = 1; // Cl
        break;
    default:
        printf("Nenhum elemento correspondente");
        break;
    }

    pedido->adicionadoElemento1 = false;
    pedido->adicionadoElemento2 = false;
}

int gamePlay(ALLEGRO_DISPLAY* display, Player* player, int numPlayers) {

    // Inicializa??o do Allegro
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

    // Define o t?tulo da janela
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
    ALLEGRO_FONT* fonteSemiBold = al_load_ttf_font("fontes/Exo-SemiBold.ttf", 15, 0);
    ALLEGRO_FONT* fonte2 = al_load_ttf_font("fontes/Exo-ExtraLight.ttf", 20, 0);
    ALLEGRO_FONT* fonte3 = al_load_ttf_font("fontes/Exo-SemiBold.ttf", 20, 0);

    // Cria as m?sicas
    ALLEGRO_SAMPLE* msc_timer = al_load_sample("msc/mscTemporizador.mp3");
    ALLEGRO_SAMPLE* msc_game = al_load_sample("msc/mscJogo.mp3");
    al_reserve_samples(2);

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

    int frame = 0; // Vari?vel para controlar o quadro atual da esteira
    int delay_ms = 1; // Atraso de 1 milissegundo entre as renderiza??es de todo o jogo
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

    // Cron?metro
    ALLEGRO_BITMAP* cronometro = al_load_bitmap("img/cronometro.png");

    // Cursor do Mouse (Seta)
    ALLEGRO_MOUSE_CURSOR* customCursor = al_create_mouse_cursor(al_load_bitmap("img/simpleCursor.png"), 0, 0);
    if (!customCursor) {
        printf("Falha ao criar o cursor da seta personalizada.\n");
        return -1;
    }

    // Cursor do Mouse (M?o)
    ALLEGRO_MOUSE_CURSOR* handCustomCursor = al_create_mouse_cursor(al_load_bitmap("img/handCursor.png"), 0, 0);
    if (!handCustomCursor) {
        printf("Falha ao criar o cursor da m?o personalizado.\n");
        return -1;
    }

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
    ////////////////////////////////////////////////////////////////////////////


    int lastPotion = -1; // Valor que n?o representa nenhuma po??o
    int consecutivePotion = 0; // Contador para po??es consecutivas
    srand(time(NULL));

    // Carregamento das imagens das po??es
    PotionProperties bitmapPotions[8];
    Potion potions[6];

    // Inicializa??o das po??es
    for (int i = 0; i < 8; i++) {
        char nomeImg[50];
        snprintf(nomeImg, sizeof(nomeImg), "img/potion%d.png", i + 1);
        bitmapPotions[i].bitmap = al_load_bitmap(nomeImg);
        bitmapPotions[i].id = pegarIndexPocao(&nomeImg) - 1;
        // Pega o index da po??o em rela??o ao seu endere?o de imagem
        // Subtrai 1 do resultado pois o array come?a em 0 e atribui ao id do bitmap da po??o

        if (i < 6)
        {
            potions[i].bitmap = bitmapPotions[i].bitmap;
            potions[i].id = bitmapPotions[i].id;

            if (!potions[i].bitmap) {
                printf("Falha ao carregar a imagem da po??o %d.\n", i + 1);
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

            potions[i].x = -150 - i * 150; // Define a posi??o inicial fora da tela e espa?amento entre as po??es
            potions[i].y = 390; // Posi??o vertical
            potions[i].velocidade = 0.25; // Velocidade das po??es
            potions[i].taArrastando = false;
        }
    }

    // Carregamento dos Pedidos
    ALLEGRO_BITMAP* bitmapOrders[5];
    Orders orders[5];

    /*
    H2O - ?gua
    NaCl - Sal de cozinha (Cloreto de s?dio)
    HCl - ?cido clor?drico
    NaOH - Soda c?ustica (Hidr?xido de s?dio)
    CO2 - G?s carb?nico (Di?xido de carbono)
    C6H12O6 - Glicose
    Fe2O3 - Ferrugem (?xido de ferro III)
    C2H5OH - ?lcool et?lico
    SO2 - G?s sulfuroso (Di?xido de enxofre)
    CH4 - Metano
    NH3 - Am?nia
    SiO2 - Areia (Di?xido de sil?cio)
    C3H6O - Acetona
    */

    char* nomesPedidosAleatorios[5];

    // Inicializando os Pedidos
    for (int i = 0; i < 5; i++) {
        char nomeImg[50];
        snprintf(nomeImg, sizeof(nomeImg), "img/pedido%d.png", i + 1);
        bitmapOrders[i] = al_load_bitmap(nomeImg);
        orders[i].propPedidos.bitmap = bitmapOrders[i];
        int indiceNomeAleatorio;
        do {
            indiceNomeAleatorio = rand() % 5;
        } while (nomeUsado[indiceNomeAleatorio]);

        nomeUsado[indiceNomeAleatorio] = true;
        orders[i].textoPedido = nomesPedidos[indiceNomeAleatorio];
        orders[i].indicePedido = indiceNomeAleatorio;
        pegarFormulaPedido(indiceNomeAleatorio, &orders[i]);

        if (!orders[i].propPedidos.bitmap) {
            printf("Falha ao carregar o pedido %d\n", i + 1);
            for (int j = 0; j < i; j++) {
                al_destroy_bitmap(bitmapOrders[j]);
            }
            return -1;
        }

        orders[i].propPedidos.x = 300 - i * 70;
        orders[i].propPedidos.y = 18;
        orders[i].adicionadoElemento1 = false;
        orders[i].adicionadoElemento2 = false;
    }

    int ordem_indices[5] = { 0, 1, 2, 3, 4 };

    for (int i = 4; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = ordem_indices[i];
        ordem_indices[i] = ordem_indices[j];
        ordem_indices[j] = temp;
    }

    memset(nomeUsado, false, sizeof(nomeUsado));

    // Mapeamento entre ?ndices de po??es e tipos correspondentes
    enum TipoPocao tipoPorIndice[8] = { NA, CL, OH, AL, MG, H, H2, O };

    // Vari?veis para o drag and drop
    bool arrastando = false;  // Indica se uma po??o est? sendo arrastada
    int indice_PocaoArrastada = -1;  // ?ndice da po??o sendo arrastada
    int deslocamentoX, deslocamentoY;  // Deslocamento do mouse em rela??o ao canto superior esquerdo da po??o arrastada
    int lastPosition = 5; // ?ndice da ?ltima po??o que ser? instanciada
    bool taArrastando = false;

    int pontuacao = 0; // Defina a pontua??o inicial
    bool metadeCompleta[5] = { false, false, false, false, false };


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
            // Verifica se o usu?rio fechou a janela no X
            if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                sair = true;
            }
            else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {  // Bot?o do mouse pressionado
                al_set_mouse_cursor(display, handCustomCursor); // Configura o cursor da m?ozinha com o bot?o pressionado
                if (event.mouse.button == 1) {  // Bot?o esquerdo do mouse pressionado
                    int mouseX = event.mouse.x;
                    int mouseY = event.mouse.y;

                    // Verifica se o mouse est? sobre alguma po??o (come?ando da ?ltima para a primeira)
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
            else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {  // Bot?o do mouse solto

                al_set_mouse_cursor(display, customCursor);  // Configura o cursor com o bot?o do mouse solto
                if (event.mouse.button == 1 && arrastando) {  // Bot?o esquerdo do mouse solto
                    arrastando = false;

                    potions[indice_PocaoArrastada].taArrastando = false;
                    potions[indice_PocaoArrastada].velocidade = 0.25;

                    if (event.mouse.x >= 620 && event.mouse.x <= (620 + al_get_bitmap_width(caldeirao)) &&
                        event.mouse.y >= 340 && event.mouse.y <= (340 + al_get_bitmap_height(caldeirao))) {
                        // A po??o foi solta dentro do caldeir?o, verificar se a combina??o est? correta
                        int tipoPocaoArrastada = tipoPorIndice[potions[indice_PocaoArrastada].id];

                        // Bool pra verificar se o jogador pegou uma pocao que nao tem em nenhum pedido
                        bool pocaoErrada = false;

                        // Verifica se a po??o corresponde a alguma combina??o correta
                        for (int i = 0; i < 5; i++)
                        {
                            // Verifica se a po??o arrastada existe naquele pedido e define true caso exista
                            if (orders[i].idElemento1 == tipoPocaoArrastada)
                            {
                                orders[i].adicionadoElemento1 = true;
                                pocaoErrada = true;
                                metadeCompleta[i] = true;

                            }
                            else if (orders[i].idElemento2 == tipoPocaoArrastada)
                            {
                                orders[i].adicionadoElemento2 = true;
                                pocaoErrada = true;
                                metadeCompleta[i] = true;

                            }

                            // Verifica se o pedido ja esta completo, ou seja, se ambos 
                            // os elementos que compoe o pedido foram coletados
                            if (orders[i].adicionadoElemento1 && orders[i].adicionadoElemento2)
                            {
                                metadeCompleta[i] = false;
                                pontuacao += 50;

                                // Percorre os pedidos pra que se caso exista um elemento que seja
                                // igual ao pedido que foi coletado, para reiniciar as suas variaveis.
                                // Ou seja, coletei um NaCl e em outro pedido tenho um NaOH, ele vai 
                                // deixar o Na desse outro pedido como falso, 
                                // fazendo com que o jogador tenha que coleta o Na novamente
                                for (int j = 0; j < 5; j++)
                                {
                                    if (j != i)
                                    {
                                        if (orders[j].idElemento1 == orders[i].idElemento1 &&
                                            orders[j].idElemento2 == orders[i].idElemento2)
                                        {
                                            orders[j].adicionadoElemento1 = false;
                                            orders[j].adicionadoElemento2 = false;
                                            metadeCompleta[j] = false;
                                        }
                                        else if (orders[j].idElemento1 == orders[i].idElemento1) {
                                            orders[j].adicionadoElemento1 = false;
                                            metadeCompleta[j] = false;
                                        }

                                        else if (orders[j].idElemento2 == orders[i].idElemento2) {
                                            orders[j].adicionadoElemento2 = false;
                                            metadeCompleta[j] = false;
                                        }
                                    }

                                }

                                // Gera um novo texto pro pedido coletado
                                orders[i].textoPedido = gerarNovoTexto(orders[i].indicePedido);
                                orders[i].indicePedido = ultimoIndiceEscolhido;
                                pegarFormulaPedido(ultimoIndiceEscolhido, &orders[i]);

                                break;
                            }
                        }

                        if (!pocaoErrada) {
                            pontuacao -= 20;
                        }
                        else
                            pocaoErrada = false;
                    }
                    else {
                        // A po??o foi solta fora do caldeir?o
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

        // Desenha o caldeir?o
        al_draw_bitmap(caldeirao, 620, 340, 0);

        // Desenha o varal 
        al_draw_bitmap(varal, -13, 20, 0);

        // Desenha a imagem atual da esteira
        al_draw_bitmap(esteira[frame], 400, 470, 0);
        al_draw_bitmap(esteira[frame], 300, 470, 0);
        al_draw_bitmap(esteira[frame], 125, 470, 0);
        al_draw_bitmap(esteira[frame], -50, 470, 0);

        // Atualiza a posi??o das po??es e desenha elas
        for (int i = 0; i < 6; i++) {
            potions[i].x += potions[i].velocidade;
            // Se a po??o sair da tela ? direita, reposicione todas em ordem aleat?ria
            if (potions[i].x > 530 && !potions[i].taArrastando) {
                int randomPotion = rand() % 8;

                // Verifica se a nova po??o ? igual ? anterior
                if (randomPotion == lastPotion) {
                    consecutivePotion++;
                }
                else {
                    consecutivePotion = 0; // Reinicia o contador se for diferente
                }

                // Se tiver 2 po??es consecutivas iguais, escolhe outra aleat?ria
                if (consecutivePotion == 2) {
                    do {
                        randomPotion = rand() % 8;
                    } while (randomPotion == lastPotion);
                    consecutivePotion = 0;
                }

                // Relacionando aquele numero randomico com a po??o e o id dela
                potions[i].bitmap = bitmapPotions[randomPotion].bitmap;
                potions[i].id = bitmapPotions[randomPotion].id;

                // Define o espa?amento de acordo com a posi??o da po??o arrastada anteriormente
                potions[i].x = potions[lastPosition].x - 150;
                lastPosition = i;

                potions[i].y = 390;
                lastPotion = randomPotion;
            }

            al_draw_bitmap(potions[i].bitmap, potions[i].x, potions[i].y, 0);
        }

        // Desenha os pedidos
        for (int i = 0; i < 5; i++) {
            // Se o pedido n?o estiver parte completo, desenha normalmente; caso contr?rio, desenha metade verde
            int metade_esquerda = al_get_bitmap_width(bitmapOrders[i]) / 2;

            if (orders[i].adicionadoElemento1)
            {
                al_draw_bitmap(bitmapOrders[ordem_indices[i]], orders[i].propPedidos.x, orders[i].propPedidos.y, 0);
                al_draw_tinted_bitmap_region(bitmapOrders[ordem_indices[i]],
                    al_map_rgba_f(1.04, 1.95, 1.53, 1.0), 0, 0, metade_esquerda,
                    al_get_bitmap_height(bitmapOrders[i]),
                    orders[i].propPedidos.x,
                    orders[i].propPedidos.y, 0);
            }
            else if (orders[i].adicionadoElemento2)
            {
                al_draw_bitmap(bitmapOrders[ordem_indices[i]], orders[i].propPedidos.x, orders[i].propPedidos.y, 0);
                al_draw_tinted_bitmap_region(bitmapOrders[ordem_indices[i]], al_map_rgba_f(1.04, 1.95, 1.53, 1.0),
                    metade_esquerda, 0, metade_esquerda, al_get_bitmap_height(bitmapOrders[i]), orders[i].propPedidos.x + metade_esquerda,
                    orders[i].propPedidos.y, 0);
            }
            else
            {
                al_draw_bitmap(bitmapOrders[ordem_indices[i]], orders[i].propPedidos.x, orders[i].propPedidos.y, 0);
            }

            /*if (!metadeCompleta[i]) {
                al_draw_bitmap(bitmapOrders[ordem_indices[i]], orders[i].propPedidos.x, orders[i].propPedidos.y, 0);
            }
            else {
                int metade_esquerda = al_get_bitmap_width(bitmapOrders[i]) / 2;
                al_draw_bitmap(bitmapOrders[ordem_indices[i]], orders[i].propPedidos.x, orders[i].propPedidos.y, 0);
                al_draw_tinted_bitmap_region(bitmapOrders[ordem_indices[i]], al_map_rgba_f(1.04, 1.95, 1.53, 1.0), metade_esquerda, 0, metade_esquerda, al_get_bitmap_height(bitmapOrders[i]), orders[i].propPedidos.x + metade_esquerda, orders[i].propPedidos.y, 0);
            }*/

            al_draw_text(fonteSemiBold, al_map_rgb(0, 0, 0),
                orders[i].propPedidos.x + al_get_bitmap_width(bitmapOrders[i]) / 2 + 4,
                orders[i].propPedidos.y + 35, ALLEGRO_ALIGN_CENTER, orders[i].textoPedido);
        }

        // Temporizador
        al_draw_bitmap(cronometro, 480, 35, 0);
        al_draw_text(fonte, al_map_rgb(255, 255, 255), 600, 42, ALLEGRO_ALIGN_CENTRE, "Tempo Restante - ");
        // Formata os minutos e segundos com dois d?gitos
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

        // Avan?ando para a pr?xima imagem da esteira
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
        orders[i].propPedidos.bitmap = NULL;
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
    al_destroy_font(fonteSemiBold);
    al_destroy_sample(msc_timer);
    al_destroy_sample(msc_game);
    al_destroy_event_queue(timer_queue);
    al_destroy_mouse_cursor(handCustomCursor);
    al_destroy_mouse_cursor(customCursor);
    al_destroy_event_queue(event_queue);
    //al_destroy_display(display);

    return 0;
}