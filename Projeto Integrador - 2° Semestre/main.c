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


int main() {
	// Inicialização do Allegro	
	if (!al_init()) {
		fprintf(stderr, "Falha ao inicializar o Allegro.\n");
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

	Player players;

	inicio(&players);

	return 0;
}
