#ifndef GAMES_H
#define GAMES_H

class Card {
	int id;
	bool visibility;
	int page;
  public:
    void set_id (int);
    void set_page (int);
	void set_visibility(bool);
    int get_id() {return id;}
    int get_page() {return page;}
    int get_visibility() {return visibility;}
};

class Game {
	Card cards[4][4];
  public:
	void init_game();
	Card* getCard(int x, int y) { return &cards[x][y]; }
};

#endif