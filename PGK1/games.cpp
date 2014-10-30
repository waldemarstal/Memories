#include "games.h"

#include <stdio.h>
#include <cstdlib>
#include <ctime>


void Card::set_id (int curr_id) {
  id = curr_id;
}

void Card::set_page (int curr_page) {
  page = curr_page;
}

void Card::set_visibility (bool curr_visibility) {
  visibility = curr_visibility;
}

void Game::init_game () {
	int rand_id;
	int count[8] = {
		0, 0, 0, 0, 0, 0, 0, 0 
	};
	srand( time(NULL) );
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4;){
			rand_id = rand() % 8;
			if (count[rand_id] < 2){
				count[rand_id]++;
				cards[i][j].set_id(rand_id);
				cards[i][j].set_visibility(true);
				cards[i][j].set_page(1);
				++j;
			}
		}
	}
}