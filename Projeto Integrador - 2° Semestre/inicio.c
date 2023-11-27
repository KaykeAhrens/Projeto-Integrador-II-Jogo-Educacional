#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/keyboard.h>
#include <allegro5/allegro_primitives.h>
#include "game.h"
#include "inicio.h"
#include "global.h"

// Tela de BemVindo
void drawBemvindo(ALLEGRO_FONT* fonte, ALLEGRO_BITMAP* logo, ALLEGRO_BITMAP* bemvindo) {
	//al_clear_to_color(al_map_rgb(255, 165, 0));
	al_draw_bitmap(bemvindo, 0, 0, 0);
	al_draw_bitmap(logo, 160, -50, 0);
	al_draw_text(fonte, al_map_rgb(255, 255, 255), 400, 330, ALLEGRO_ALIGN_CENTRE, "Bem-vindo ao jogo!");
	al_draw_text(fonte, al_map_rgb(255, 255, 255), 400, 380, ALLEGRO_ALIGN_CENTRE, "Digite seu nome:");
	al_draw_filled_rectangle(302, 438, 500, 470, al_map_rgb(255, 255, 255));
	al_flip_display();
}

// Desenha nome do jogador
void drawNome(const char* playerName, ALLEGRO_FONT* fonte2) {
	al_draw_text(fonte2, al_map_rgb(0, 0, 0), 400, 440, ALLEGRO_ALIGN_CENTRE, playerName);
}

// Desenha opções de voltar ou sair
void displayOptions(ALLEGRO_FONT* fonte2) {
	al_draw_text(fonte2, al_map_rgb(255, 255, 255), 800 / 2, 510, ALLEGRO_ALIGN_CENTRE, "Pressione V para Voltar ou S para Sair.");
	al_flip_display();
}

// Função para desenhar um losango com bordas arredondadas e o número da colocação

void drawLosango(int x, int y, int size, int number, ALLEGRO_FONT* fonte3) {
	ALLEGRO_COLOR color = al_map_rgb(31, 161, 171); // Cor do losango

	// Pontos para desenhar um losango
	float points[] = {
		x, y - size / 2,          // Ponto superior
		x - size / 2, y,          // Ponto esquerdo
		x, y + size / 2,          // Ponto inferior
		x + size / 2, y           // Ponto direito
	};


	// Desenha os triângulos com bordas arredondadas
	for (int i = 0; i < 4; ++i) {
		al_draw_filled_triangle(
			points[(i * 2) % 8], points[(i * 2 + 1) % 8],
			points[((i + 1) * 2) % 8], points[((i + 1) * 2 + 1) % 8],
			x, y, color
		);

		al_draw_line(
			points[i * 2], points[i * 2 + 1],
			points[((i + 1) * 2) % 8], points[((i + 1) * 2 + 1) % 8],
			al_map_rgb(31, 161, 171), 2.0
		);

		al_draw_line(
			x, y,
			points[i * 2], points[i * 2 + 1],
			al_map_rgb(31, 161, 171), 2.0
		);

		al_draw_line(
			x, y,
			points[((i + 1) * 2) % 8], points[((i + 1) * 2 + 1) % 8],
			al_map_rgb(31, 161, 171), 2.0
		);
	}

	// Ajusta a posição do texto
	float textWidth = al_get_text_width(fonte3, "99"); // Máximo de dois dígitos
	float textHeight = al_get_font_line_height(fonte3);
	float textX = x - textWidth / 2; // Ajusta para o centro
	float textY = y - textHeight / 2; // Ajusta para o centro

	// Desenha o número no centro do losango
	if (number >= 2 || number <= 3 || number == 7) {
		al_draw_textf(fonte3, al_map_rgb(255, 255, 255), textX + 12, textY, ALLEGRO_ALIGN_CENTER, "%d", number);
	}
	al_draw_textf(fonte3, al_map_rgb(255, 255, 255), textX + 11.5, textY, ALLEGRO_ALIGN_CENTER, "%d", number);
}

// Função para desenhar a imagem de sombra
void drawShadow(int x, int y, ALLEGRO_BITMAP* shadow) {
	al_draw_bitmap(shadow, x, y, 0);
}

int inicio() {
	// Inicialização do Allegro	
	if (!al_init()) {
		fprintf(stderr, "Falha ao inicializar o Allegro.\n");
		return -1;
	}

	// Criação do display
	ALLEGRO_DISPLAY* display = al_create_display(800, 600);
	if (!display) {
		printf("Falha ao criar janela.\n");
		return -1;

	}

	// Inicialização dos add-ons necessários
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_primitives_addon();
	al_install_keyboard();
	al_init_font_addon();
	al_init_image_addon();

	// Carrega a imagem da logo do jogo
	ALLEGRO_BITMAP* logo = al_load_bitmap("img/logo.png");
	if (!logo) {
		fprintf(stderr, "Falha ao carregar a imagem da logo!\n");
		return -1;

	}
	// Carrega a imagem de fundo do placar
	ALLEGRO_BITMAP* background = al_load_bitmap("img/fundo_placar.png");
	if (!background) {
		fprintf(stderr, "Falha ao carregar a imagem de fundo!\n");
		return -1;
	}
	// Carrega e desenha a imagem de fundo da tela BemVindo
	ALLEGRO_BITMAP* bemvindo = al_load_bitmap("img/fundo_bemvindo.png");
	if (!background) {
		fprintf(stderr, "Falha ao carregar a imagem de fundo!\n");
		return -1;
	}

	// Carrega a imagem de sombra
	ALLEGRO_BITMAP* shadow = al_load_bitmap("img/sombra.png");
	if (!shadow) {
		fprintf(stderr, "Falha ao carregar a imagem de sombra!\n");
		return -1;
	}


	// Criação da fonte
	ALLEGRO_FONT* font = al_create_builtin_font();
	ALLEGRO_FONT* fonte = al_load_ttf_font("fontes/Exo-SemiBold.ttf", 30, 0);
	ALLEGRO_FONT* fonte2 = al_load_ttf_font("fontes/Exo-ExtraLight.ttf", 20, 0);
	ALLEGRO_FONT* fonte3 = al_load_ttf_font("fontes/Exo-SemiBold.ttf", 20, 0);


	// Criação da fila de eventos e registro do teclado como fonte de eventos
	ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	//// Desenha tela inicial 
	drawBemvindo(fonte, logo, bemvindo);
	al_flip_display();

	// Aceita no máx 3 jogadores, podemos mudar
	Player players;

	// Loop para cada jogador
	while (numPlayers < 10) {
		int pos = 0;
		while (true) {
			ALLEGRO_EVENT ev;
			al_wait_for_event(event_queue, &ev);

			// Desenha tela inicial com nome do jogador 
			drawBemvindo(fonte, logo, bemvindo);
			drawNome(players.name, fonte2);

			al_flip_display();

			// Lógica para entrada do nome do jogador
			if (ev.type == ALLEGRO_EVENT_KEY_CHAR) {
				if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER) {
					++numPlayers;
					gamePlay(display, &players, numPlayers);
					break;
				}
				else if (pos < 49 && ev.keyboard.unichar >= 32 && ev.keyboard.unichar <= 126) { // verifica tecla para digitar
					players.name[pos] = (char)ev.keyboard.unichar;
					players.name[pos + 1] = '\0';
					pos++;
				}
				else if (ev.keyboard.keycode == ALLEGRO_KEY_BACKSPACE && pos > 0) { // verifica tecla para deletar
					players.name[--pos] = '\0';
				}
			}
		}
		//placar(players, numPlayers, background, shadow, logo, fonte2, fonte3, display);
	}

	al_destroy_display(display);

	return 0;
}